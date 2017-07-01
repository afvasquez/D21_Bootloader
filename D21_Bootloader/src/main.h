/*
 * main.h
 *
 * Created: 6/28/2017 3:00:22 PM
 *  Author: avasquez
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "usb_protocol_cdc.h"

/*! \brief Opens the communication port
 * This is called by CDC interface when USB Host enable it.
 *
 * \retval true if cdc startup is successfully done
 */
bool main_cdc_enable(uint8_t port);

/*! \brief Closes the communication port
 * This is called by CDC interface when USB Host disable it.
 */
void main_cdc_disable(uint8_t port);

/*! \brief Save new DTR state to change led behavior.
 * The DTR notify that the terminal have open or close the communication port.
 */
void main_cdc_set_dtr(uint8_t port, bool b_enable);

/*! \brief Called by CDC interface
 * Callback running when CDC device have received data
 */

void main_cdc_rx_notify(uint8_t port);

/*! \brief Configures communication line
 *
 * \param cfg      line configuration
 */
void main_cdc_set_coding(uint8_t port, usb_cdc_line_coding_t * cfg);

#endif /* MAIN_H_ */