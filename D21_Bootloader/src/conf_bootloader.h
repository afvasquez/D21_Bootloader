/*
 * conf_bootloader.h
 *
 * Created: 6/28/2017 10:51:53 AM
 *  Author: avasquez
 */ 


#ifndef CONF_BOOTLOADER_H_INCLUDED
#define CONF_BOOTLOADER_H_INCLUDED

#include "conf_board.h"

#define APP_START_ADDRESS          0x00006000
#define BOOT_LED		PIN_PA28
#define BOOT_ACTIVITY	PIN_PA07

#define MOTOR_PIN_PWM	PIN_PA14
#define MOTOR_PIN_AH	PIN_PA15
#define MOTOR_PIN_BH	PIN_PA16
#define MOTOR_PIN_CH	PIN_PA17
#define MOTOR_PIN_AL	PIN_PA18
#define MOTOR_PIN_BL	PIN_PA19
#define MOTOR_PIN_CL	PIN_PA22

#endif /* CONF_BOOTLOADER_H_ */