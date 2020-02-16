// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include <lcom/timer.h>
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"


extern uint8_t mouseByte[3];
extern bool mouseSync;
extern int mouseByteCounter;
extern uint32_t timerCounter;
extern bool triggered;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {

  int ipc_status, r;
  message msg;

  uint8_t bit_no = 1;
  uint32_t irq_set;

  if(mouse_enable_data_report() != OK) return 1;

  if(mouse_subscribe_int(&bit_no)!= OK) return 1;

  irq_set = BIT(bit_no);
  
  while(cnt != 0){ /* You may want to use a different condition */
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
                mouse_ih();
                
                if (mouseByteCounter == 3)
                {
                  struct packet info;
                  read_packet(&info);
                  mouse_print_packet(&info);

                  mouseByteCounter = 0;
                  cnt--;
                }

                if (!mouseSync)
                mouseByteCounter = 0;

                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }

  }

  if(mouse_unsubscribe_int(&bit_no) != OK) return 1;

  if(mouse_disable_data_report() != OK) return 1;

  return 0;

}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    uint8_t data = 0;
    
    for(int i = 0; i < cnt; i++) {
      for (int counter = 0; counter<3; counter++){

        if(write_mouse(MOUSE_READ_DATA) != OK) return 1;
        if(util_sys_inb(KBC_OUT_BUF, &data)!= OK) return 1;

        mouseByte[counter] = data;
        
        if(counter == 2){
          mouseByte[2] = data;
          
          struct packet info;
          read_packet(&info);
          mouse_print_packet(&info);

        }
      }
      tickdelay(micros_to_ticks(period*1000));
    }

    if(minix_default_cmd_byte()!= OK) return 1;

    if(write_mouse(MOUSE_SET_STREAM_MODE)!= OK) return 1;
    
    if(mouse_disable_data_report()!= OK) return 1;

    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
   int ipc_status, r;
  message msg;

  uint8_t mouse_bit_no = 1;
  uint32_t mouse_irq_set;

  if(mouse_enable_data_report() != OK) return 1;

  if(mouse_subscribe_int(&mouse_bit_no)!= OK) return 1;
  mouse_irq_set = BIT(mouse_bit_no);
  
  uint8_t timer_bit_no = 0;
  uint32_t timer_irq_set;

  if(timer_subscribe_int(&timer_bit_no)!= 0) return 1;
  timer_irq_set = BIT(timer_bit_no);
  
  int n = idle_time;
  while(idle_time != 0){ /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & mouse_irq_set) 
                { /* subscribed interrupt */
                mouse_ih();
                
                if (mouseByteCounter == 3)
                {
                  struct packet info;
                  read_packet(&info);
                  mouse_print_packet(&info);

                  mouseByteCounter = 0;
                timerCounter = 0;
                idle_time = n;
                }

                if (!mouseSync)
                mouseByteCounter = 0;

                }

                if(msg.m_notify.interrupts & timer_irq_set)
                {
                  timer_int_handler();
                  if (timerCounter == 0) 
                  {
                  timer_print_elapsed_time();
                  idle_time--;
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

  if(mouse_unsubscribe_int(&mouse_bit_no) != OK) return 1;

  if(mouse_disable_data_report() != OK) return 1;

  return 0;
}


int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance)
{
   int ipc_status, r;
  message msg;
  uint8_t bit_no = 12;
  uint32_t irq_set;

  if(mouse_enable_data_report() != OK) return 1;

  if(mouse_subscribe_int(&bit_no)!= OK) return 1;

  irq_set = BIT(bit_no);
  
  while(triggered){ /* You may want to use a different condition */
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
                mouse_ih();
                
                if (mouseByteCounter == 3)
                {
                  struct packet info;
                  read_packet(&info);
                  mouse_print_packet(&info);
                  struct mouse_ev *ev = mouse_event_handler(&info);
                  state_machine_draw_mouse(ev, x_len, tolerance);

                  mouseByteCounter = 0;
                }

                if (!mouseSync)
                mouseByteCounter = 0;

                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }

  if(mouse_unsubscribe_int(&bit_no) != OK) return 1;

  if(mouse_disable_data_report() != OK) return 1;

  return 0;
}

