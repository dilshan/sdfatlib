#ifndef SD_DEFINITIONS_HEADER
#define SD_DEFINITIONS_HEADER

// Command mnemonics for SD/MMC.
#define SD_CMD0     0   // GO_IDLE_STATE.
#define SD_CMD1     1   // SEND_OP_COND (MMC).
#define SD_CMD8     8   // SEND_IF_COND.
#define SD_CMD12    12  // STOP_TRANSMISSION.
#define SD_CMD16    16  // SET_BLOCKLEN.
#define SD_CMD17    17  // READ_SINGLE_BLOCK.
#define SD_CMD18    18  // READ_MULTIPLE_BLOCK.
#define SD_CMD24    24  // WRITE_BLOCK.
#define SD_CMD25    25  // WRITE_MULTIPLE_BLOCK.
#define SD_CMD55    55  // APP_CMD.
#define SD_CMD58    58  // READ_OCR.

#define SD_ACMD41   169 // SEND_OP_COND (SDC).
#define SD_ACMD23   151 // SET_WR_BLK_ERASE_COUNT (SDC).

#endif /* SD_DEFINITIONS_HEADER */