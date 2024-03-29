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
 * $LastChangedRevision: 133 $
 */

/** @file
 * @brief UART example
 * @defgroup uart_example UART "Hello World" example
 * @{
 * @ingroup nrf_examples
 *
 * @brief This example writes the string "Hello World" on start-up. After this all
 * characters received on the RXD input are echoed to the TXD output.
 *
 * The example implements the low level stdio functions putchar() and getchar() so that standard
 * IO functions such as printf() and gets() can be used by the application.
 *
*/

//lint -e732
//lint -e713
//lint -e640

#include <stdio.h>
#include "nrf24le1.h"
#include "hal_uart.h"
#include "hal_clk.h"

// Cusomization of low level stdio function. Used by for example printf().
#ifdef __ICC8051__
int putchar(int c)
#else /*presume C51 or other accepting compilator*/
char putchar(char c)
#endif
{
  hal_uart_putchar(c);
  return c;
}

// Cusomization of low level stdio function. Used by for example gets().
#ifdef __ICC8051__
int getchar(void)
#else /*presume C51 or other accepting compilator*/
char getchar(void)
#endif
{
  return hal_uart_getchar();
}

// Repeated putchar to print a string
void putstring(char *s)
{
  while(*s != 0)
    putchar(*s++);
}

void main(void)
{
  // Configure TXD pin as output.
  // P0.5, P0.3 and P1.0 are configured as outputs to make the example run on
  // either 24-pin, 32-pin or 48-pin nRF24LE1 variants.
  P0DIR = 0xD7;
  P1DIR = 0xFE;

   // Initializes the UART
  hal_uart_init(UART_BAUD_9K6);

  // Wait for XOSC to start to ensure proper UART baudrate
  while(hal_clk_get_16m_source() != HAL_CLK_XOSC16M)
  {}

  // Enable global interrupts
  EA = 1;

  // Print "Hello World" at start-up
  putstring("\r\nHello World!\r\n");

  for(;;)
  {
    // If any characters received
    if( hal_uart_chars_available() )
    {
			P3 = 0x11;
      // Echo received characters
      putchar(getchar());
    }
  }
}
/** @} */
