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
#define BOOT_LED		PIN_PA27
#define BOOT_ACTIVITY	PIN_PA07
#define BOOT_ERROR		PIN_PA28

#define MOTOR_PIN_PWM	PIN_PA14
#define MOTOR_PIN_AH	PIN_PA15
#define MOTOR_PIN_BH	PIN_PA16
#define MOTOR_PIN_CH	PIN_PA17
#define MOTOR_PIN_AL	PIN_PA18
#define MOTOR_PIN_BL	PIN_PA19
#define MOTOR_PIN_CL	PIN_PA22

#define BOOT_USART_MODULE          SERCOM0
#define BOOT_USART_BAUDRATE        115200
#define BOOT_USART_MUX_SETTINGS    PINMUX_UNUSED
#define BOOT_USART_PAD0            PINMUX_UNUSED
#define BOOT_USART_PAD1            PINMUX_UNUSED
#define BOOT_USART_GCLK_SOURCE     GCLK_GENERATOR_0

#define APP_START_PAGE             (APP_START_ADDRESS / FLASH_PAGE_SIZE)

/* DEBUG LED output enable/disable */
#define DEBUG_ENABLE               false

#endif /* CONF_BOOTLOADER_H_ */