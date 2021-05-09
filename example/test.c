#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "../openfat/include/openfat.h"
#include "../openfat/include/openfat/mbr.h"

#include "../logger/logger.h"
#include "../sddriver/sddriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

int main()
{
	struct block_mbr_partition part;
	struct block_device dev;
	struct fat_vol_handle volHandle;
	struct fat_file_handle fileHandle;

	char statusBuff[30];

	char dirname[20];
	char filename[20];
	char buffer[2000];
	
	// Configure to use internal 48MHz clock.
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);

	// Configure PortC pin13 (onboard LED) to output mode.
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_10_MHZ , GPIO_CNF_OUTPUT_PUSHPULL , GPIO13);

	// Configure USART and SPI peripherals.
	loggerInit();
	sdSPIInit();

	// Initialize SD card driver.
	if(!sdDrvInit())
	{
		loggerOutputLine("Driver initialization fail");
		while(1);
	}

	loggerOutputLine("Driver initialization successful");

	// Initialize OpenFAT block device structure with SD driver functions.
	dev.read_sectors = sdDrvReadSector;
	dev.write_sectors = sdDrvWriteSector;
	dev.get_sector_size = sdDrvGetSectorSize;

	// Initialize partition block device.
	if(mbr_partition_init(&part, &dev, 0) == 0)
	{
		loggerOutputLine("mbr_partition_init successful");
	}
	else
	{
		loggerOutputLine("mbr_partition_init fail");
		while(1);
	}

	// Initialize and mount the FAT volume.
	if(fat_vol_init((struct block_device *)&part, &volHandle) == 0)
	{
		loggerOutputLine("fat_vol_init successful");
		sprintf(statusBuff, "FAT type = %d", volHandle.type);
		loggerOutputLine(statusBuff);

	}
	else
	{
		loggerOutputLine("fat_vol_init fail");
		while(1);
	}

	// Start directory and file creation test (moved form example.c in original OpenFAT project).
	loggerOutputLine("Start Dir/File test...");

	for(int i = 0; i < 5; i++) {
		sprintf(dirname, "Test%d", i);
		fat_mkdir(&volHandle, dirname);
		assert(fat_chdir(&volHandle, dirname) == 0);

		for(int j = 0; j < 5; j++) {
			sprintf(filename, "File%d", j);
			assert(fat_create(&volHandle, filename, O_WRONLY, &fileHandle) == 0);
			assert(fat_write(&fileHandle, buffer, sizeof(buffer)) == sizeof(buffer));
		}

		assert(fat_chdir(&volHandle, "..") == 0);
	}	

	loggerOutputLine("End Dir/File test...");
	// End of directory and file creation test.

	while(1)
	{
		gpio_toggle(GPIOC, GPIO13);
		for(long p = 0; p < 0xFFFFFF; p++);
	}

	return 0;
}
