// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include <lcom/timer.h>
#include "graphics.h"
#include "keyboard.h"
#include "i8042.h"

extern uint8_t scanCode[2];
bool check = false;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if(vg_init_function(mode) != OK) return 1;
  tickdelay(micros_to_ticks(delay*1000000));
  if(vg_exit() != OK) return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  
  if(vg_init_function(mode) != OK) return 1;
  
  draw_rectangle(x, y, width, height, color);
  
  int ipc_status;
  message msg;
  int r;

  uint8_t bit_no = 0;
  uint32_t irq_set;

  if(keyboard_subscribe_int(&bit_no) != OK) return 1;

  irq_set = BIT(bit_no);
  
  while(scanCode[0] != ESC_BREAK_CODE){ /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) 
                { /* subscribed interrupt */
                
                kbc_ih();
                
                if (scanCode[0] == 0xE0){
                  if (check) 
                    check = false;
                  else 
                    check = true;
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
  if(keyboard_unsubscribe_int()!= OK) return 1;

  if(vg_exit() != OK) return 1;
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(vg_init_function(mode) != OK) return 1;
  draw_pattern(mode, no_rectangles, first, step);
  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no = 0;
  uint32_t irq_set;
  if(keyboard_subscribe_int(&bit_no)!= OK) return 1;
  irq_set = BIT(bit_no);
  
  while(scanCode[0] != ESC_BREAK_CODE){ /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) 
                { /* subscribed interrupt */
                
                kbc_ih();
                
                if (scanCode[0] == 0xE0){
                  if (check) 
                    check = false;
                  else 
                    check = true;
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
  if(keyboard_unsubscribe_int()!= OK) return 1;
  if(vg_exit() != OK) return 1;
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);
  //Para começar, deve-se colocar o video mode em 0x105
 
  if(vg_init_function(0x105) != OK) return 1; //Forçar que o modo de inicialização seja 0x105
  //xpm_load() || xpm_read();                 //Perguntar ao stor qual dela é para se usar Converte                                               um XPM num pixelmap
  
  xpm_image_t img;
  enum xpm_image_type type = XPM_INDEXED;
  uint8_t *sprite = xpm_load(xpm,type,&img);
  uint8_t width = img.width;
  uint8_t height = img.height;
  draw_xpm(sprite, x, y, width, height);
  
  int ipc_status;
  message msg;
  int r;
  uint8_t bit_no = 0;
  uint32_t irq_set;
  //Tudo aqui até keyboard unsubscribe serve para sair quando pressionar (ESC)
  if(keyboard_subscribe_int(&bit_no)!= OK) return 1;
  irq_set = BIT(bit_no);
  
  while(scanCode[0] != ESC_BREAK_CODE){ /* You may want to use a different condition */
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) 
                { /* subscribed interrupt */
                kbc_ih();
                if (scanCode[0] == 0xE0){
                  if (check) 
                    check = false;
                  else 
                    check = true;
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
  if(keyboard_unsubscribe_int()!= OK) return 1;
  //Sair do modo de video para o modo de texto
  if(vg_exit() != OK) return 1;
  return 0;
        
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
 
  if(vg_init_function(0x105) != OK) return 1;
  xpm_image_t img;
  enum xpm_image_type type = XPM_INDEXED;
  uint8_t *sprite = xpm_load(xpm,type,&img);
  uint16_t x = xi;
  uint16_t y = yi;
  uint8_t width = img.width;
  uint8_t height = img.height;
  draw_xpm(sprite, x, y, width, height);
  int16_t frame_counter = 0;
 
  int ipc_status;
  message msg;
  int r;
  uint8_t kbc_bit_no = 0, timer_bit_no = 1;
  uint32_t kbc_irq_set, timer_irq_set;
  if(keyboard_subscribe_int(&kbc_bit_no)!= 0) return 1;
  kbc_irq_set = BIT(kbc_bit_no);
  if(timer_subscribe_int(&timer_bit_no)!= 0) return 1;
  timer_irq_set = BIT(timer_bit_no);
  uint32_t counter = 0;
  while((scanCode[0] != ESC_BREAK_CODE)){ /* You may want to use a different condition */
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
                  counter++;          
 
                  if(speed < 0){
                    if((counter % (60/fr_rate)) == 0) {
                        frame_counter += 1;
                      if(x < xf){
                        if(frame_counter == -speed){
                          x+= 1;
                          frame_counter = 0;
                        }
                        clear_screen();
                        draw_xpm(sprite, x, y, width, height);
                       
                      }
                      if(y < yf) {
                        if(frame_counter == -speed){
                          y+= 1;
                          frame_counter = 0;
                        }
                        clear_screen();
                        draw_xpm(sprite, x, y, width, height);    
                      }
                      if(x > xf){
                        if(frame_counter == -speed){
                          x-= 1;
                          frame_counter = 0;
                        }
                        clear_screen();
                        draw_xpm(sprite, x, y, width, height);
                       
                      }
                      if(y > yf) {
                        if(frame_counter == -speed){
                          y-= 1;
                          frame_counter = 0;
                        }
                        clear_screen();
                        draw_xpm(sprite, x, y, width, height);    
                      }
                    }
                  }
                  else{
                    if((counter % (60/fr_rate)) == 0) {
                     
                      if(xi < xf){
                        if((x+speed) < xf){
                          x += speed;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                        else if (x != xf) {
                          x = xf;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                        if((y+speed) < yf) {
                          y += speed;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                        else if(y != yf) {
                          y = yf;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                      }
                      else if(xi > xf){
                        if((x-speed) > xf){
                          x -= speed;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                        else if (x != xf) {
                          x = xf;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                        if((y-speed) > yf) {
                          y -= speed;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                        else if(y != yf) {
                          y = yf;
                          clear_screen();
                          draw_xpm(sprite, x, y, width, height);
                        }
                      }
                    }
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
  if(vg_exit() != OK) return 1;
  return 0;
}

int(video_test_controller)() {
  vg_vbe_contr_info_t info_p;
  memset(&info_p, 0, sizeof(vg_vbe_contr_info_t));

  read_vbe(&info_p);

  vg_display_vbe_contr_info(&info_p);

  return 1;
}
