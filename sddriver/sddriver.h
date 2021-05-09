#ifndef SDDRIVER_MAIN_HEADER
#define SDDRIVER_MAIN_HEADER

#include "../openfat/include/openfat.h"

void sdSPIInit();
int8_t sdDrvInit();

static uint8_t sdDrvCommand(uint8_t cmd, uint32_t arg);
static int sdDrvGetBlock(uint8_t *buf, int len);
static int sdDrvSetBlock(const uint8_t *buf, int len, uint8_t token);

int sdDrvReadSector(const struct block_device *bldev, uint32_t sector, uint32_t count, void *buf);
int sdDrvWriteSector(const struct block_device *bldev, uint32_t sector, uint32_t count, const void *buf);
uint16_t sdDrvGetSectorSize(const struct block_device *dev);

#endif /* SDDRIVER_MAIN_HEADER */