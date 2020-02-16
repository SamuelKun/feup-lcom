#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"

#ifdef LAB3
extern uint32_t sysCounter;
#endif

extern uint32_t timerCounter;
extern uint8_t scanCode[2];

bool check = false;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() 
{
  int ipc_status;
  message msg;
  int r;

  uint8_t bit_no = 0;
  uint32_t irq_set;

  if(keyboard_subscribe_int(&bit_no)!= 0) return 1;

  irq_set = BIT(bit_no);
  
  while(scanCode[0] != ESC_BREAK_CODE){ /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) 
                { /* subscribed interrupt */
                
                kbc_ih();
                
                if (scanCode[0] == 0xE0)
                {
                  if (check)
                  {
                  kbd_print_scancode(keyboard_makecode(scanCode[1]), 2, scanCode);
                  check = false;
                  }
                  else check = true;
                }
                else
                 {
                    kbd_print_scancode(keyboard_makecode(scanCode[0]), 1, scanCode);
                }
                }    
                   
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
 }

  if(keyboard_unsubscribe_int()!= 0) return 1;

  kbd_print_no_sysinb(sysCounter);
  
  return 0;
}

int(kbd_test_poll)() 
{
  uint8_t statusByte;
  while(scanCode[0] != ESC_BREAK_CODE) 
  {
    if(util_sys_inb(KBC_STAT_REG, &statusByte)!= 0) return 1;
    if((statusByte & KBC_OBF))
      {
      kbc_ih();        

      if (scanCode[0] == 0xE0)
      {
        if (check)
        {
        kbd_print_scancode(keyboard_makecode(scanCode[1]), 2, scanCode);
        check = false;
        }
        else check = true;
      }
      else
        {
          kbd_print_scancode(keyboard_makecode(scanCode[0]), 1, scanCode);
      }
      
      tickdelay(micros_to_ticks(KBC_DELAY_US));
      }
  }    

  keyboard_reenable();

  return 0;
  }


int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status;
  message msg;
  int r;
  uint8_t kbc_bit_no = 0, timer_bit_no = 1;
  uint32_t kbc_irq_set, timer_irq_set;

  if(keyboard_subscribe_int(&kbc_bit_no)!= 0) return 1;
  kbc_irq_set = BIT(kbc_bit_no);
  if(timer_subscribe_int(&timer_bit_no)!= 0) return 1;
  timer_irq_set = BIT(timer_bit_no);

  int time = n;
  while((scanCode[0] != ESC_BREAK_CODE) && (time != 0)){ /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */                                
                if (msg.m_notify.interrupts & kbc_irq_set) 
                { /* subscribed interrupt */
                
                kbc_ih();
                time = n;
                timerCounter = 0;
                if (scanCode[0] == 0xE0)
                {
                  if (check)
                  {
                  kbd_print_scancode(keyboard_makecode(scanCode[1]), 2, scanCode);
                  
                  check = false;
                  }
                  else check = true;
                }
                else
                {
                    kbd_print_scancode(keyboard_makecode(scanCode[0]), 1, scanCode);
                }
                
                }    
                if(msg.m_notify.interrupts & timer_irq_set)
                {
                  timer_int_handler();
                  if (timerCounter == 0) 
                  {
                  timer_print_elapsed_time();
                  time--;
                  }  

                    
                } 
                break;
            default:
                break; /* no other notifications expected: do nothing */        
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }
  if(keyboard_unsubscribe_int()!= 0) return 1;
  if(timer_unsubscribe_int()!= 0) return 1;
  return 0;
}
