#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard and Mouse.
 */


#define KBC_CMD_REG   0x64    /**< Command Register */
#define KBC_STAT_REG  0x64    /**< Status Register  */
#define KBC_OUT_BUF   0x60    /**< Output Buffer    */
#define KBC_IN_BUF    0x60    /**< Input buffer     */


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

//PS/2 Mouse
#define MOUSE_IRQ 12      //Mouse IRQ line

//Byte 1
#define Y_OVERFLOW BIT(7)         // X axis overflow
#define X_OVERFLOW BIT(6)         // Y axis overflow
#define MSB_Y_DELTA BIT(5)        // 
#define MSB_X_DELTA BIT(4)        //
#define ALWAYS_1 BIT(3)           // This bit must always be 1 to prevent errors
#define MIDDLE_MOUSE_BUTTON BIT(2)// Press middle mouse button
#define RIGHT_MOUSE_BUTTON BIT(1) // Press right mouse button
#define LEFT_MOUSE_BUTTON BIT(0)  // Press left mouse button

#define MOUSE_BYTE 0xD4
//Commands passed as arguments of commmand 0xD4
//Response to these commands, if any, are put in the KBC's OUT_BUF and should be read via port 0x60
#define MOUSE_RESET 0XFF          //Mouse reset
#define MOUSE_RESEND 0XFE           //For serial communication errors
#define MOUSE_SET_DEFAULTS 0XF6         //Set default values
#define MOUSE_DISABLE_REPORTING 0XF5    //In stream mode, should be sent before any other command
#define MOUSE_ENABLE_REPORTING 0XF4     //In stream mode only
#define MOUSE_SET_SAMPLE_RATE 0XF3      //Sets state sampling rate
#define MOUSE_SET_REMOTE_MODE 0XF0      //Send data on request only
#define MOUSE_READ_DATA 0XEB            //Send data packet request
#define MOUSE_SET_STREAM_MODE 0xEA      //Send data on events
#define MOUSE_STATUS_REQUEST 0XE9       //Get mouse configuration(3 bytes) 
#define MOUSE_SET_RESOLUTION 0XE8       
#define MOUSE_SET_SCALING_21 0XE7       //Acceleration mode
#define MOUSE_SET_sACLING_11 0XE6       //Linear mode
#define MOUSE_ACK 0xFA

/**@}*/

#endif /* _LCOM_I8042_H */

