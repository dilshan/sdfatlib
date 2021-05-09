#ifndef SD_SPI_MAIN_HEADER
#define SD_SPI_MAIN_HEADER

void sdCardIntfInit();

uint8_t sdSPIreadwrite(uint8_t data);

void sdSPISelect();
void sdSPIRelease();

void sdSPIWriteBuffer(const uint8_t *buf, int len);
void sdSPIReadBuffer(uint8_t *buf, int len);

#endif /* SD_SPI_MAIN_HEADER */