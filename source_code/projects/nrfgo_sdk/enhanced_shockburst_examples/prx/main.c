/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic
 * Semiconductor ASA.Terms and conditions of usage are described in detail
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * $LastChangedRevision: 2211 $
 */

/** @file
 * @brief Enhanced ShockBurst Primary Receiver example
 * @defgroup esb_prx_example Enhanced ShockBurst Primary Receiver (PRX) example
 * @{
 * @ingroup nrf_examples
 *
 * @brief This example monitors for data and writes the first byte (byte 0) of the
 * received payloads to P0.
 *
 * The example shows the minimum required setup for receiving packets from a
 * primary transmitter (PTX) device.
 *
 * The following default radio parameters are being used:
 * - RF channel 2
 * - 2 Mbps data rate
 * - RX address 0xE7E7E7E7E7 (pipe 0) and 0xC2C2C2C2C2 (pipe 1)
 * - 1 byte CRC
 *
 * The project @ref esb_ptx_example can be used as a counterpart for transmitting the data.
 *
*/

//lint -e717
//lint -e534
//lint -e714
//lint -e783

#ifdef MCU_NRF24LE1
#include "nrf24le1.h"
#include "hal_clk.h"
#endif

#ifdef MCU_NRF24LU1P
#include "nrf24lu1p.h"
#endif

#include <stdint.h>
#include "hal_nrf.h"

// Global variables
uint8_t payload[3];

void main()
{
#ifdef MCU_NRF24LE1
  while(hal_clk_get_16m_source() != HAL_CLK_XOSC16M)
  {
    // Wait until 16 MHz crystal oscillator is running
  }
#endif
  
  #ifdef MCU_NRF24LU1P
  // Enable radio SPI
  RFCTL = 0x10;
  #endif

  // Set P0 as output
  P0DIR = 0;

  // Enable the radio clock
  RFCKEN = 1;

  // Enable RF interrupt
  RF = 1;
  // Enable global interrupt
  EA = 1;

  // Configure radio as primary receiver (PTX)
  hal_nrf_set_operation_mode(HAL_NRF_PRX);

  // Set payload width to 3 bytes
  hal_nrf_set_rx_payload_width((int)HAL_NRF_PIPE0, 3);

  // Power up radio
  hal_nrf_set_power_mode(HAL_NRF_PWR_UP);

  // Enable receiver
  CE_HIGH();

  for(;;){}
}

// Radio interrupt
NRF_ISR()
{
  uint8_t irq_flags;

  // Read and clear IRQ flags from radio
  irq_flags = hal_nrf_get_clear_irq_flags();

  // If data received
  if((irq_flags & (1<<(uint8_t)HAL_NRF_RX_DR)) > 0)
  {
    // Read payload
    while(!hal_nrf_rx_fifo_empty())
    {
      hal_nrf_read_rx_payload(payload);
    }

    // Write received payload[0] to port 0
    P0 = payload[0];
  }
}
/** @} */
