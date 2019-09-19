/** \file vs10xx.h
 * Headers for interfacing with the mp3 player chip.
 * Interfacing the New Way, not handling BSYNC -> not compatible with VS1001.
 */


#ifndef VS10XX_H
#define VS10XX_H


/** VS10xx SCI Write Command byte is 0x02 */
#define VS_WRITE_COMMAND 0x02

/** VS10xx SCI Read Command byte is 0x03 */
#define VS_READ_COMMAND 0x03


#define SPI_MODE	0x0   /**< VS10xx register */
#define SPI_STATUS	0x1   /**< VS10xx register */
#define SPI_BASS	0x2   /**< VS10xx register */
#define SPI_CLOCKF	0x3   /**< VS10xx register */
#define SPI_DECODE_TIME	0x4   /**< VS10xx register */
#define SPI_AUDATA	0x5   /**< VS10xx register */
#define SPI_WRAM	0x6   /**< VS10xx register */
#define SPI_WRAMADDR	0x7   /**< VS10xx register */
#define SPI_HDAT0	0x8   /**< VS10xx register */
#define SPI_HDAT1	0x9   /**< VS10xx register */
#define SPI_AIADDR	0xa   /**< VS10xx register */
#define SPI_VOL		0xb   /**< VS10xx register */
#define SPI_AICTRL0	0xc   /**< VS10xx register */
#define SPI_AICTRL1	0xd   /**< VS10xx register */
#define SPI_AICTRL2	0xe   /**< VS10xx register */
#define SPI_AICTRL3	0xf   /**< VS10xx register */

#define SM_DIFF		0x01   /**< VS10xx register */
#define SM_JUMP		0x02   /**< VS10xx register */
#define SM_RESET	0x04   /**< VS10xx register */
#define SM_OUTOFWAV	0x08   /**< VS10xx register */
#define SM_PDOWN	0x10   /**< VS10xx register */
#define SM_TESTS	0x20   /**< VS10xx register */
#define SM_STREAM	0x40   /**< VS10xx register */
#define SM_PLUSV	0x80   /**< VS10xx register */
#define SM_DACT		0x100   /**< VS10xx register */
#define SM_SDIORD	0x200   /**< VS10xx register */
#define SM_SDISHARE	0x400   /**< VS10xx register */
#define SM_SDINEW	0x800   /**< VS10xx register */
#define SM_ADPCM        0x1000   /**< VS10xx register */
#define SM_ADPCM_HP     0x2000   /**< VS10xx register */



/** Put the MP3 player chip in reset */
#define Mp3PutInReset(){MP3_XRESET=0;}

/** Release the MP3 player chip from reset */
#define Mp3ReleaseFromReset(){MP3_XRESET=1;}


/** Pull the VS10xx Control Chip Select line Low */
#define Mp3SelectControl(){MP3_XCS=0;}

/** Pull the VS10xx Control Chip Select line High */
#define Mp3DeselectControl(){MP3_XCS=1;}

/** Pull the VS10xx Data Chip Select line Low */
#define Mp3SelectData(){MP3_XDCS=0;}

/** Pull the VS10xx Data Chip Select line High */
#define Mp3DeselectData(){MP3_XDCS=1;}


#ifndef _IN_VS10XX_
extern void Mp3WriteRegister(uint8 addressbyte,uint8 highbyte,uint8 lowbyte);
extern void Mp3SetVolume(uint8 leftchannel,uint8 rightchannel);
extern unsigned int Mp3ReadRegister (unsigned char addressbyte);
extern void VS1003Init(void);
extern void VsSineTest();
extern void VS1003SendDat(uint8 dat);
extern void VS1003FlushBuffer(void);


#endif

#endif
