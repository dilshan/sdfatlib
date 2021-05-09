/******************************************************************************
 * SD FAT library for STM32 ARM Cortex-M3 MCUs.
 * Copyright (C) 2021 Dilshan R Jayakody [jayakody2000lk@gmail.com]
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 *****************************************************************************
 * This library is based on OpenFAT project written by 
 * Gareth McMullin <gareth@blacksphere.co.nz>. 
 * <https://github.com/tmolteno/openfat>
 *****************************************************************************/

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "logger.h"

// USART peripheral to provide debug logs.
#define USART_DEV           USART2

// USART output port.
#define USART_PORT          GPIOA

// USART output port pin (TX).
#define USART_PORT_TX       GPIO_USART2_TX

// USART related clock configurations.
#define USART_CLK_ID                RCC_USART2
#define USART_OUPUT_PORT_CLK_ID     RCC_GPIOA

// Baud rate to capture the debug logs.
#define USART_BAUD_RATE     115200


#ifdef DEBUG_LOG
// Debug logs are enabled in this build configuration.

void loggerInit()
{
    // Enable peripheral clocks of the USART related I/O port.
    rcc_periph_clock_enable(USART_OUPUT_PORT_CLK_ID);

    // Enable peripheral clocks for USART.
    rcc_periph_clock_enable(USART_CLK_ID);

    gpio_set_mode(USART_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, USART_PORT_TX);

    // Set USART to provide debug outputs with default baud rate with 8N1 configuration.
    usart_set_baudrate(USART_DEV, USART_BAUD_RATE);
  	usart_set_databits(USART_DEV, 8);
  	usart_set_stopbits(USART_DEV, USART_STOPBITS_1);
  	usart_set_mode(USART_DEV, USART_MODE_TX_RX);
  	usart_set_parity(USART_DEV, USART_PARITY_NONE);
  	usart_set_flow_control(USART_DEV, USART_FLOWCONTROL_NONE);

    // Enable USART2 as debug output.
    usart_enable(USART_DEV);
}

void loggerOutput(char *log)
{
    while(*log)
    {
        usart_send_blocking(USART_DEV, *log);
        log++;
    }
}

void loggerOutputLine(char *log) 
{
    loggerOutput(log);
    loggerOutput("\r\n");
}

#else
// Debug logs are disabled in this build configuration.

void loggerInit() {}
void loggerOutput(char *log) {}
void loggerOutputLine(char *log) {}

// End of DEBUG_LOG.
#endif