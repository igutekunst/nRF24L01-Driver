#ifndef NRF_H
#define NRF_H
#include "nRF24L01_config.h"
#define SET_MASK(value,mask) value |= mask
#define CLEAR_MASK(value,mask) value &= (~mask)
#define CLR_MASK(u,v) CLEAR_MASK(u,v)

/////////////////////////////////////////////////////////////////////////////////////
// To use this library with hardware interrupts, simply place a call to nrfTick()  //
// Inside the ISR
// Otherwise, call nrfTick() if you want to use callbacks
////////////////////////////////////////////////////////////////////////////////////


#define NRF_CSN_SET() NRF_CSN_LOW()
#define NRF_CSN_CLEAR() NRF_CSN_HIGH()
#define NRF_CE_SET() NRF_CE_HIGH()
#define NRF_CE_CLEAR() NRF_CE_LOW()


/**
 * Initialize the nrf library 
 * Set's up the CSN and CE pins using definitions in nRF24L01_config.h
 */
void nrfInit();


/**
 * Write one byte to the NRF chip
 */
unsigned char nrfWrite(unsigned char c);

/*
 * Write multiple bytes to the NRF chip from *data
 * Returns number of bytes written
 */
unsigned char nrfWriteBulk(unsigned char *value, unsigned int len);

/*
 * Read a byte from the NRF chip
 */
unsigned char nrfRead(unsigned char c);

/*
 * Read multiple bytes from the NRF chip into *data
 */
unsigned char nrfReadBulk(unsigned char *value, unsigned int len);

/**
 * Write a single byte to one of the NRF registers
 */
unsigned char nrfWriteReg(unsigned char reg, unsigned char value);
/*
 * Write a multibyte register to the NRF chip from *buffer
#define NRF_SETUP_AW_AW_MASK (0x3)
 */
unsigned char nrfWriteMultibyteReg(unsigned char reg, unsigned char *value, int len);
/*
 * read a register from the NRF
 */
unsigned char nrfReadReg(unsigned char reg);

/*
 * read a multibyte register from the NRF chip
 */
unsigned char nrfReadMultibyteReg(unsigned char reg, unsigned char *value, unsigned int len);

void nrfFlushTx();

void nrfFlushRx();

void nrfReuseTxPl();
void nrfPowerUp();
void nrfActivate(unsigned char rx_active);
/**
 * Instructs the library to call callback when data is ready to be recieved.
 * If interrupts are enabled, this will happen almost immediately. Otherwise, this will occur
 * somtime during a call to nrfTick() which polls the interrupt flags on the nrf chip
 *
 */
void nrfInitRxCallback(void (*callback)(unsigned char data, 
                                          unsigned char len,
                                          unsigned char pipe_nun));

    
/**
 * Set a callback when data has been sent.
 * With enhanced Shockburst, this will only happen if an ACK was received
 * 
 */
void nrfInitTxCallback(void (*callback)(void));

/**
 * Set a callback when the TX fifo is full
 */
void nrfInitTxFullCallback( void (*callback)());





void nrfWriteTxPayload(char * data, unsigned int len);

void nrfReadRxPayload(char * data, unsigned int len);

unsigned char nrfReadRxPayloadWidth();

/*
 *Writes data in an acknowledgment packet
 */
void nrfWriteTxPayloadAck(char * data, int len);

/*
 * Writes to tx buffer, and disables ack for this packet
 */
void nrfWriteTxPayloadNoAck(char * data, int len);

/**
 * Puts chip in receiver mode
 */
void nrfSetRxMode();
/**
 * Puts chip in transmitter mode
 */
void nrfSetTxMode();
//SPI Commands

/**
 * Read command and status registers.
 * AAAAA = 5 bit Register Map Address
 */
#define NRF_SPI_R_REGISTER          (0x00)

/**
 * Write command and status registers.
 * AAAAA = 5 bit Register Map Address
 * Executable in power down or standby modes only.
 */
#define NRF_SPI_W_REGISTER          (0x20)

/**
 * Read RX-payload: 1 – 32 bytes.
 * A read operation always starts at byte 0.
 * Payload is deleted from FIFO after it is read.
 * Used in RX mode.
 * Data LSB first
 */
#define NRF_SPI_R_RX_PAYLOAD        (0x61)

/**
 * Write TX-payload: 1 – 32 bytes.
 * A write operation always starts at byte 0 used in TX payload.
 */
#define NRF_SPI_W_TX_PAYLOAD        (0xA0)

/**
 * Flush TX FIFO,
 * Used in TX mode
 */
#define NRF_SPI_FLUSH_TX            (0xE1)
/**
 * Flush RX FIFO, used in RX mode
 * Should not be executed during transmission of acknowledge,
 * that is, acknowledge package will not be completed.
 */
#define NRF_SPI_FLUSH_RX            (0xE2)
/**
 * Used for a PTX device
 * Reuse last transmitted payload.
 * Packets are repeatedly retransmitted as long as CE is high.
 * TX payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed.
 * TX payload reuse must not be activated or deacti- vated during package transmission
 */
#define NRF_SETUP_AW_AW_MASK (0x3)
#define NRF_SPI_REUSE_TX_PL         (0xE3)
/**
 * Read RX-payload width for the top R_RX_PAYLOAD in the RX FIFO.
 */
#define NRF_SPI_R_RX_LP_WID         (0x60)

/**
 * Used in RX mode.
Write Payload to be transmitted together with ACK packet on PIPE PPP.
 * (PPP valid in the range from 000 to 101).
 * Maximum three ACK packet payloads can be pending.
 * Payloads with same PPP are handled using first in - first out principle.
 * Write payload: 1– 32 bytes. A write operation always starts at byte 0.
 */
#define NRF_SPI_W_ACK_PAYLOAD       (0xA8)
/**
 * Used in TX mode. Disables AUTOACK on this specific packet.
 */
#define NRF_SPI_W_TX_PAYLOAD_NOACK  (0xB0)
/**
 * No Operation. Might be used to read the STATUS register
 */
#define NRF_SPI_NOP                 (0xFF)

#define TRASH 0xFE

#define NRF_CONFIG		0x00
#define NRF_EN_AA		0x01
#define NRF_EN_RXADDR		0x02
#define NRF_SETUP_AW		0x03
#define NRF_SETUP_RETR		0x04
#define NRF_RF_CH		0x05
#define NRF_RF_SETUP		0x06
#define NRF_STATUS		0x07
#define NRF_OBSERVE_TX		0x08
#define NRF_CD			0x09
#define NRF_RX_ADDR_P0		0x0A
#define NRF_RX_ADDR_P1		0x0B
#define NRF_RX_ADDR_P2		0x0C
#define NRF_RX_ADDR_P3		0x0D
#define NRF_RX_ADDR_P4		0x0E
#define NRF_RX_ADDR_P5		0x0F
#define NRF_TX_ADDR		0x10
#define NRF_RX_PW_P0		0x11
#define NRF_RX_PW_P1		0x12
#define NRF_RX_PW_P2		0x13
#define NRF_RX_PW_P3		0x14
#define NRF_RX_PW_P4		0x15
#define NRF_RX_PW_P5		0x16

////////////////////////////////////////////////////////////////////////////////////
// Register bitwise definitions
//
// Below are the defines for each register's bitwise fields in the 24L01.
////////////////////////////////////////////////////////////////////////////////////
//CONFIG register bitwise definitions
#define NRF_CONFIG_RESERVED		0x80
#define	NRF_CONFIG_MASK_RX_DR		0x40
#define	NRF_CONFIG_MASK_TX_DS		0x20
#define	NRF_CONFIG_MASK_MAX_RT		0x10
#define	NRF_CONFIG_EN_CRC		0x08
#define	NRF_CONFIG_CRCO			0x04
#define	NRF_CONFIG_PWR_UP		0x02
#define	NRF_CONFIG_PRIM_RX		0x01

//EN_AA register bitwise definitions
#define NRF_EN_RXADDR_ERX_ALL		0x3F
#define NRF_EN_RXADDR_ERX_P5		0x20
#define NRF_EN_RXADDR_ERX_P4		0x10
#define NRF_EN_RXADDR_ERX_P3		0x08
#define NRF_EN_RXADDR_ERX_P2		0x04
#define NRF_EN_RXADDR_ERX_P1		0x02
#define NRF_EN_RXADDR_ERX_P0		0x01
#define NRF_EN_RXADDR_ERX_NONE		0x00

//SETUP_AW register bitwise definitions
#define NRF_SETUP_AW_RESERVED		0xFC
#define NRF_SETUP_AW			0x03
#define NRF_SETUP_AW_5BYTES		0x03
#define NRF_SETUP_AW_4BYTES		0x02
#define NRF_SETUP_AW_3BYTES		0x01
#define NRF_SETUP_AW_ILLEGAL		0x00

//SETUP_RETR register bitwise definitions
#define NRF_SETUP_RETR_ARD		0xF0
#define NRF_SETUP_RETR_ARD_4000		0xF0
#define NRF_SETUP_RETR_ARD_3750		0xE0
#define NRF_SETUP_RETR_ARD_3500		0xD0
#define NRF_SETUP_RETR_ARD_3250		0xC0
#define NRF_SETUP_RETR_ARD_3000		0xB0
#define NRF_SETUP_RETR_ARD_2750		0xA0
#define NRF_SETUP_RETR_ARD_2500		0x90
#define NRF_SETUP_RETR_ARD_2250		0x80
#define NRF_SETUP_RETR_ARD_2000		0x70
#define NRF_SETUP_RETR_ARD_1750		0x60
#define NRF_SETUP_RETR_ARD_1500		0x50
#define NRF_SETUP_RETR_ARD_1250		0x40
#define NRF_SETUP_RETR_ARD_1000		0x30
#define NRF_EN_AA_RESERVED		0xC0
#define NRF_EN_AA_ENAA_ALL		0x3F
#define NRF_EN_AA_ENAA_P5		0x20
#define NRF_EN_AA_ENAA_P4		0x10
#define NRF_EN_AA_ENAA_P3		0x08
#define NRF_EN_AA_ENAA_P2		0x04
#define NRF_EN_AA_ENAA_P1		0x02
#define NRF_EN_AA_ENAA_P0		0x01
#define NRF_EN_AA_ENAA_NONE		0x00

//EN_RXADDR register bitwise definitions
// Enabled RX address
#define NRF_EN_RXADDR_RESERVED		0xC0
#define NRF_EN_RXADDR_ERX_ALL		0x3F
#define NRF_EN_RXADDR_ERX_P5		0x20
#define NRF_EN_RXADDR_ERX_P4		0x10
#define NRF_EN_RXADDR_ERX_P3		0x08
#define NRF_EN_RXADDR_ERX_P2		0x04
#define NRF_EN_RXADDR_ERX_P1		0x02
#define NRF_EN_RXADDR_ERX_P0		0x01
#define NRF_EN_RXADDR_ERX_NONE		0x00

//SETUP_AW register bitwise definitions
#define NRF_SETUP_AW_RESERVED		0xFC
#define NRF_SETUP_AW			0x03
#define NRF_SETUP_AW_5BYTES		0x03
#define NRF_SETUP_AW_4BYTES		0x02
#define NRF_SETUP_AW_3BYTES		0x01
#define NRF_SETUP_AW_ILLEGAL		0x00

//SETUP_RETR register bitwise definitions
#define NRF_SETUP_RETR_ARD		0xF0
#define NRF_SETUP_RETR_ARD_4000		0xF0
#define NRF_SETUP_RETR_ARD_3750		0xE0
#define NRF_SETUP_RETR_ARD_3500		0xD0
#define NRF_SETUP_RETR_ARD_3250		0xC0
#define NRF_SETUP_RETR_ARD_3000		0xB0
#define NRF_SETUP_RETR_ARD_2750		0xA0
#define NRF_SETUP_RETR_ARD_2500		0x90
#define NRF_SETUP_RETR_ARD_2250		0x80
#define NRF_SETUP_RETR_ARD_2000		0x70
#define NRF_SETUP_RETR_ARD_1750		0x60
#define NRF_SETUP_RETR_ARD_1500		0x50
#define NRF_SETUP_RETR_ARD_1250		0x40
#define NRF_SETUP_RETR_ARD_1000		0x30
#define NRF_SETUP_RETR_ARD_750		0x20
#define NRF_SETUP_RETR_ARD_500		0x10
#define NRF_SETUP_RETR_ARD_250		0x00
#define NRF_SETUP_RETR_ARC		0x0F
#define NRF_SETUP_RETR_ARC_15		0x0F
#define NRF_SETUP_RETR_ARC_14		0x0E
#define NRF_SETUP_RETR_ARC_13		0x0D
#define NRF_SETUP_RETR_ARC_12		0x0C
#define NRF_SETUP_RETR_ARC_11		0x0B
#define NRF_SETUP_RETR_ARC_10		0x0A
#define NRF_SETUP_RETR_ARC_9		0x09
#define NRF_SETUP_RETR_ARC_8		0x08
#define NRF_SETUP_RETR_ARC_7		0x07
#define NRF_SETUP_RETR_ARC_6		0x06
#define NRF_SETUP_RETR_ARC_5		0x05
#define NRF_SETUP_RETR_ARC_4		0x04
#define NRF_SETUP_RETR_ARC_3		0x03
#define NRF_SETUP_RETR_ARC_2		0x02
#define NRF_SETUP_RETR_ARC_1		0x01
#define NRF_SETUP_RETR_ARC_0		0x00

//RF_CH register bitwise definitions
#define NRF_RF_CH_RESERVED		0x80

//RF_SETUP register bitwise definitions
#define NRF_RF_SETUP_RESERVED		0xE0
#define NRF_RF_SETUP_PLL_LOCK		0x10
#define NRF_RF_SETUP_RF_DR		0x08
#define NRF_RF_SETUP_RF_PWR		0x06
#define NRF_RF_SETUP_RF_PWR_0		0x06
#define NRF_RF_SETUP_RF_PWR_6 		0x04
#define NRF_RF_SETUP_RF_PWR_12		0x02
#define NRF_RF_SETUP_RF_PWR_18		0x00
#define NRF_RF_SETUP_LNA_HCURR		0x01

//STATUS register bitwise definitions
#define NRF_STATUS_RESERVED			0x80
#define NRF_STATUS_RX_DR			0x40
#define NRF_STATUS_TX_DS			0x20
#define NRF_STATUS_MAX_RT			0x10
#define NRF_STATUS_RX_P_NO			0x0E
#define NRF_STATUS_RX_P_NO_RX_FIFO_NOT_EMPTY	0x0E
#define NRF_STATUS_RX_P_NO_UNUSED		0x0C
#define NRF_STATUS_RX_P_NO_5			0x0A
#define NRF_STATUS_RX_P_NO_4			0x08
#define NRF_STATUS_RX_P_NO_3			0x06
#define NRF_STATUS_RX_P_NO_2			0x04
#define NRF_STATUS_RX_P_NO_1			0x02
#define NRF_STATUS_RX_P_NO_0			0x00

#define NRF_STATUS_TX_FULL			0x01

//OBSERVE_TX register bitwise definitions
#define NRF_OBSERVE_TX_PLOS_CNT		0xF0
#define NRF_OBSERVE_TX_ARC_CNT		0x0F

//CD register bitwise definitions
#define NRF_CD_RESERVED			0xFE
#define NRF_CD_CD			0x01

//RX_PW_P0 register bitwise definitions
#define NRF_RX_PW_P0_RESERVED		0xC0

//RX_PW_P1 register bitwise definitions
#define NRF_RX_PW_P1_RESERVED		0xC0

//RX_PW_P2 register bitwise definitions
#define NRF_RX_PW_P2_RESERVED		0xC0

//RX_PW_P3 register bitwise definitions
#define NRF_RX_PW_P3_RESERVED		0xC0

//RX_PW_P4 register bitwise definitions
#define NRF_RX_PW_P4_RESERVED		0xC0

//RX_PW_P5 register bitwise definitions
#define NRF_RX_PW_P5_RESERVED		0xC0

//FIFO_STATUS register bitwise definitions
#define NRF_FIFO_STATUS_RESERVED	0x8C
#define NRF_FIFO_STATUS_TX_REUSE	0x40
#define NRF_FIFO_STATUS_TX_FULL	0x20
#define NRF_FIFO_STATUS_TX_EMPTY	0x10
#define NRF_FIFO_STATUS_RX_FULL	0x02
#define NRF_FIFO_STATUS_RX_EMPTY	0x01




#endif
