#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */


#define KBC_CMD_REG 0x64   //Command Register
#define KBC_STAT_REG 0x64  //Status Register
#define KBC_OUT_BUF 0x60   //Output Buffer
#define KBC_IN_BUF 0x60    //Input buffer


#define READ_COMMAND_BYTE 0X20      //Returns Command Byte
#define WRITE_COMMAND_BYTE 0x60     //Takes A: Command Byte
#define CHECK_KBC 0xAA              //Returns 0x55, if OK || Returns 0xFC, if error
#define CHECK_KEYBOARD_INTEFACE 0xAB//Returns 0, if OK
#define DISABLE_KBD_INTERFACE 0xAD  //Disables KBD Interface
#define ENABLE_KBD_INTERFACE 0xAE   //Enables KBD Interface


#define ESC_BREAK_CODE 0x81//Escape Break Code
#define KBC_IRQ 0x01       //Sets policy for KBD interrupt request
#define KBC_DELAY_US 20000 //20 ms of timer delay

#define KBC_PARITY BIT(7)  //Parity error - invalid data
#define KBC_TIMEOUT BIT(6) //Timeout error - invalid data
#define KBC_AUX BIT(5)     //Mouse data
#define KBC_INH BIT(4)     //Inhibit flag: 0 if keyboard is inhibited
#define KBC_A2 BIT(3)      //A2 input line: 0 data byte 1 command byte
#define KBC_SYS BIT(2)     //System flag: 0 if system in power-on reset, 1 if system already                                   //initialized
#define KBC_IBF BIT(1)     //Input buffer full - Don’t write commands or arguments
#define KBC_OBF BIT(0)     //Output buffer full - data available for reading

/**@}*/

#endif /* _LCOM_I8042_H */

