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
#include <libopencm3/stm32/spi.h>

#include "sddriver.h"
#include "sdspi.h"

// SPI peripheral to interface with SD card.
#define SDDRV_SPI               SPI2

// SPI related port.
#define SDDRV_SPI_PORT          GPIOB

// SPI chip selected (CS) related port.
#define SDDRV_SPI_CS_PORT       GPIOA

// SPI MOSI port pin.
#define SDDRV_SPI_MOSI          GPIO_SPI2_MOSI

// SPI MISO port pin.
#define SDDRV_SPI_MISO          GPIO_SPI2_MISO

// SPI SCK port pin.
#define SDDRV_SPI_SCK           GPIO_SPI2_SCK

// SPI chip select (CS) port pin.
#define SDDRV_SPI_CS            GPIO8

// SPI related clock configurations.
#define SDDRV_SPI_PORT_CLK_ID       RCC_GPIOB
#define SDDRV_SPI_CS_PORT_CLK_ID    RCC_GPIOA
#define SDDRV_SPI_CLK_ID            RCC_SPI2

void sdSPIInit()
{
    // Enable peripheral clocks for SPI related I/O ports.
    rcc_periph_clock_enable(SDDRV_SPI_PORT_CLK_ID);
    rcc_periph_clock_enable(SDDRV_SPI_CS_PORT_CLK_ID);
    
    // Enable peripheral clocks for SPI to handle SD card interface.
    rcc_periph_clock_enable(SDDRV_SPI_CLK_ID);

    // Configure SCK, MOSI and CS pins in output mode.
    gpio_set_mode(SDDRV_SPI_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, SDDRV_SPI_SCK | SDDRV_SPI_MOSI);
    gpio_set_mode(SDDRV_SPI_CS_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, SDDRV_SPI_CS);

    // Configure MISO pin in input mode.
    gpio_set_mode(SDDRV_SPI_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, SDDRV_SPI_MISO);
}

void sdCardIntfInit()
{    
    // Initialize SPI in master mode to perform SD card communication.
    spi_init_master(SDDRV_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_2, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    spi_enable_software_slave_management(SDDRV_SPI);
    spi_set_nss_high(SDDRV_SPI);
    spi_enable(SDDRV_SPI);
}   

uint8_t sdSPIreadwrite(uint8_t data)
{
    while(SPI_SR(SDDRV_SPI) & SPI_SR_BSY);
    SPI_DR(SDDRV_SPI) = data;

    while(!(SPI_SR(SDDRV_SPI) & SPI_SR_RXNE));
    return SPI_DR(SDDRV_SPI);
}

void sdSPISelect()
{
    gpio_clear(SDDRV_SPI_CS_PORT, SDDRV_SPI_CS);
	
    // Wait until not busy.
    while(sdSPIreadwrite(0xFF) == 0);
}

void sdSPIRelease()
{
    gpio_set(SDDRV_SPI_CS_PORT, SDDRV_SPI_CS);
    sdSPIreadwrite(0xFF);
}

void sdSPIWriteBuffer(const uint8_t *buf, int len)
{
    while(len--)
    {
        sdSPIreadwrite(*buf++);
    }
}

void sdSPIReadBuffer(uint8_t *buf, int len)
{
    while(len--)
    {
        *buf++ = sdSPIreadwrite(0xFF);
    }
}
