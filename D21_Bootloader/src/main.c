/*
 * D21_Bootloader.cpp
 *
 * Created: 6/28/2017 8:21:17 AM
 * Author : Andres Vasquez
 */ 


#include "asf.h"
#include "conf_board.h"
#include "conf_clocks.h"
#include "conf_bootloader.h"
#include "sam_ba_monitor.h"
#include "usart_sam_ba.h"

#define BOOT_ENTER_SIGNAL	port_pin_set_output_level(BOOT_LED, 1)
#define BOOT_EXIT_SIGNAL	port_pin_set_output_level(BOOT_LED, 0)

static void check_start_application(void);

#ifdef CONF_USBCDC_INTERFACE_SUPPORT
static volatile bool main_b_cdc_enable = false;
#endif

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
	port_pin_set_config(BOOT_ERROR, &config_port);

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

	udc_start();
    
	while ( !main_b_cdc_enable ) {  }

	if(main_b_cdc_enable) {
		sam_ba_monitor_init(SAM_BA_INTERFACE_USBCDC);
		// SAM-BA on USB loop 
		port_pin_set_output_level(BOOT_ACTIVITY, true);

		while(1) {
			sam_ba_monitor_run();
		}
	}

	port_pin_set_output_level(BOOT_ERROR, true);
	while (true) {
	}
}

#ifdef CONF_USBCDC_INTERFACE_SUPPORT

bool main_cdc_enable(uint8_t port)
{
	main_b_cdc_enable = true;
	return true;
}

void main_cdc_disable(uint8_t port)
{
	main_b_cdc_enable = false;
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
}

void main_cdc_rx_notify(uint8_t port)
{
}

void main_cdc_set_coding(uint8_t port, usb_cdc_line_coding_t * cfg)
{
}

#endif // CONF_USBCDC_INTERFACE_SUPPORT