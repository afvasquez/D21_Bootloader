/*
 * D21_Bootloader.cpp
 *
 * Created: 6/28/2017 8:21:17 AM
 * Author : Andres Vasquez
 */ 


#include <asf.h>
#include "conf_bootloader.h"

#define BOOT_ENTER_SIGNAL	port_pin_set_output_level(BOOT_LED, 1)
#define BOOT_EXIT_SIGNAL	port_pin_set_output_level(BOOT_LED, 0)

/**
 * \brief Check the application startup condition
 *
 */
static void check_start_application(void)
{
	uint32_t app_start_address;

	/* Load the Reset Handler address of the application */
	app_start_address = *(uint32_t *)(APP_START_ADDRESS + 4);

	/**
	 * Test reset vector of application @APP_START_ADDRESS+4
	 * Stay in SAM-BA if *(APP_START+0x4) == 0xFFFFFFFF
	 * Application erased condition
	 */
	if (app_start_address == 0xFFFFFFFF) {
		/* Stay in bootloader */
		return;
	}

	// NOTE: Pin-based bootloading was removed for this application

	/* Rebase the Stack Pointer */
	__set_MSP(*(uint32_t *) APP_START_ADDRESS);

	/* Rebase the vector table base address */
	SCB->VTOR = ((uint32_t) APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk);

	/* Leaving bootloader memory section */
	BOOT_EXIT_SIGNAL;

	/* Jump to application Reset Handler in the application */
	asm("bx %0"::"r"(app_start_address));
}

static void motor_control_shutdown(void) {
	struct port_config config_port;
	port_get_config_defaults(&config_port);
	config_port.direction = PORT_PIN_DIR_OUTPUT;

	port_pin_set_config(MOTOR_PIN_PWM, &config_port);
	port_pin_set_config(MOTOR_PIN_AH, &config_port);
	port_pin_set_config(MOTOR_PIN_BH, &config_port);
	port_pin_set_config(MOTOR_PIN_CH, &config_port);
	port_pin_set_config(MOTOR_PIN_AL, &config_port);
	port_pin_set_config(MOTOR_PIN_BL, &config_port);
	port_pin_set_config(MOTOR_PIN_CL, &config_port);
	port_pin_set_config(BOOT_LED, &config_port);
	port_pin_set_config(BOOT_ACTIVITY, &config_port);

	port_pin_set_output_level(MOTOR_PIN_PWM, 0);
	port_pin_set_output_level(MOTOR_PIN_AH, 0);
	port_pin_set_output_level(MOTOR_PIN_BH, 0);
	port_pin_set_output_level(MOTOR_PIN_CH, 0);
	port_pin_set_output_level(MOTOR_PIN_AL, 0);
	port_pin_set_output_level(MOTOR_PIN_BL, 0);
	port_pin_set_output_level(MOTOR_PIN_CL, 0);
}

int main(void)
{
	BOOT_ENTER_SIGNAL;

	// ADDED: Motor Shutdown
	motor_control_shutdown();

	/* Jump in application if condition is satisfied */
	check_start_application();

    /* Initialize the SAM system */
    system_init();

	port_pin_set_output_level(BOOT_ACTIVITY, true);
    /* Replace with your application code */
    while (1) 
    {
    }
}
