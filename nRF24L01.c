#include "nRF24L01.h"
#include <p24F16KA102.h>
#include <spi.h>
#include "delayNOP.h"


//private global variables

unsigned char address_p0[5];
unsigned char address_p1[5];
unsigned char address_p2_lsb;
unsigned char address_p3_lsb;
unsigned char address_p4_lsb;
unsigned char address_p5_lsb;

void ( *nrf_rx_callback )     (unsigned char * data, unsigned char len, unsigned char pipe_num);
void ( *nrf_tx_callback )     (void);
void ( *nrf_tx_full_callback) (void); 



void nrfInit(){
    NRF_CSN_SET_OUTPUT();
    NRF_CE_SET_OUTPUT();
  
    NRF_CSN_CLEAR(); //don't select nrf

    spiInit(0);
    
     

}


/*
 * Write one byte to the NRF chip
 */
unsigned char nrfWrite(unsigned char c){
    NRF_CSN_SET();
   
    
    NRF_SPI_TRANSFER(c);
    
    NRF_CSN_CLEAR();
  
}

/*
 * Write multiple bytes to the NRF chip from *data
 * Returns number of bytes written
 */
unsigned char nrfWriteBulk(unsigned char *value, unsigned int len){
    NRF_CSN_SET();
    unsigned int i;
    for(i=0;i<len;i++){

        nrfWrite(value[i]);
    }
    NRF_CSN_CLEAR();
}

/*
 * Read a byte from the NRF chip
 */
unsigned char nrfRead(unsigned char c){
    NRF_CSN_SET();
    unsigned char out = NRF_SPI_TRANSFER(c);

    NRF_CSN_CLEAR();
    return out;
}


/*
 * Read multiple bytes from the NRF chip into *data
 */
unsigned char nrfReadBulk(unsigned char *value, unsigned int len){
    NRF_CSN_SET();
    unsigned int  i;
    for(i=0; i<len;i++){
        value[i] = NRF_SPI_TRANSFER();
    }
    NRF_CSN_CLEAR();
}

/*
 * Write a single byte to one of the NRF registers
 */
unsigned char nrfWriteReg(unsigned char reg, unsigned char value){
    NRF_CSN_SET();
    NRF_SPI_TRANSFER(NRF_SPI_W_REGISTER | reg);
    NRF_SPI_TRANSFER(value);
    NRF_SPI_TRANSFER(TRASH);

     NRF_CSN_CLEAR();
 
}

/*
 * Write a multibyte register to the NRF chip from *buffer
 */
unsigned char nrfWriteMultibyteReg(unsigned char reg, unsigned char *value, int len){
   NRF_SPI_TRANSFER(NRF_SPI_W_REGISTER | reg);//tell the NRF we're going to write a register
   unsigned int i;
   for(i=0;i<len;i++){
       NRF_SPI_TRANSFER(value[i]);
   }
   NRF_CSN_CLEAR();
   return len;

}

/*
 * read a register from the NRF
 */
unsigned char nrfReadReg(unsigned char reg){
    NRF_CSN_SET();
    NRF_SPI_TRANSFER(NRF_SPI_R_REGISTER | reg );   //tel the NRF we are going to read a register
    
    char c =  NRF_SPI_TRANSFER(0b101010);          //get the value
    NRF_CSN_CLEAR();
    return c;


}

/*
 * read a multibyte register from the NRF chip
 */
unsigned char nrfReadMultibyteReg(unsigned char reg, unsigned char *value, unsigned int len){
    NRF_CSN_SET();
   NRF_SPI_TRANSFER(NRF_SPI_R_REGISTER | reg);//tell the NRF we're going to write a register

   unsigned int i;
   for(i=0;i<len;i++){
       value[i] = NRF_SPI_TRANSFER(TRASH);
   }
   NRF_CSN_CLEAR();
   return len;
}


unsigned char nrfSetBit(unsigned char reg, unsigned char mask){
    unsigned char value = nrfReadReg(reg);
    if(value & mask){
        return value;
    }else{
        SET_MASK(value,mask);
        nrfWriteReg(reg,value);
        return value;
    }
    
}

unsigned char nrfClearBit(unsigned char reg, unsigned char mask){
    unsigned char value = nrfReadReg(reg);
    if(value & mask==0){
        return value;
    }else{
        CLEAR_MASK(value,mask);
        nrfWriteReg(reg,value);
        return value;
    }

}

void nrfFlushTx(){
    NRF_CSN_SET();

    NRF_SPI_TRANSFER(NRF_SPI_FLUSH_TX );
    NRF_CSN_CLEAR();
    
}

void nrfFlushRx(){
    NRF_CSN_SET();

    NRF_SPI_TRANSFER(NRF_SPI_FLUSH_RX );
    NRF_CSN_CLEAR();

}

void nrfReuseTxPl(){
    NRF_CSN_SET();

    NRF_SPI_TRANSFER(NRF_SPI_REUSE_TX_PL );
    NRF_CSN_CLEAR();

}

void nrfInitRxCallback(void (*callback)(unsigned char data, 
                                          unsigned char len,
                                          unsigned char pipe_nun)){
  nrf_rx_callback = callback;

}
    
void nrfInitTxCallback(void (*callback)(void)){
  nrf_tx_callback = callback;
}

void nrfInitTxFullCallback( void (*callback)()){
  nrf_tx_full_callback = callback;

}
void nrfPowerUp(){
  unsigned char config = nrfReadReg(NRF_CONFIG);
  if(config & NRF_CONFIG_PWR_UP){
    return;
  }else{
      SET_MASK(config,NRF_CONFIG_PWR_UP);
      nrfWriteReg(NRF_CONFIG,config);
      delay(2); //wait at least 1.5 ms for power up sequence
    
  }
}
void nrfPowerDown(){
  unsigned char config = nrfReadReg(NRF_CONFIG);
  if(config & NRF_CONFIG_PWR_UP==0){       //already off
    return;
  }else{
      CLEAR_MASK(config,NRF_CONFIG_PWR_UP);
      nrfWriteReg(NRF_CONFIG,config);

  }
}


void nrfWriteTxPayload(char * data, unsigned int len){
    NRF_CSN_SET();
    NRF_SPI_TRANSFER(NRF_SPI_R_RX_PAYLOAD);
    unsigned int i;
    for(i=0;i<len;i++){
       data[i] = NRF_SPI_TRANSFER(TRASH);
    }
    NRF_CSN_CLEAR();


}

void nrfReadRxPayload(char * data, unsigned int len){
    NRF_CSN_SET();
    NRF_SPI_TRANSFER(NRF_SPI_R_RX_PAYLOAD);
    unsigned int i;
    for(i=0;i<len;i++){
       data[i] = NRF_SPI_TRANSFER(TRASH);
    }
    NRF_CSN_CLEAR();


}

unsigned char nrfReadRxPayloadWidth(){
    NRF_CSN_SET();

    char c = NRF_SPI_TRANSFER(NRF_SPI_REUSE_TX_PL );
    NRF_CSN_CLEAR();
    return c;
    
}

/*
 *Writes data in an acknowledgment packet
 */
void nrfWriteTxPayloadAck(char * data, int len){
     NRF_CSN_SET();
    NRF_SPI_TRANSFER(NRF_SPI_R_RX_PAYLOAD);
    unsigned int i;
    for(i=0;i<len;i++){
       data[i] = NRF_SPI_TRANSFER(TRASH);
    }
    NRF_CSN_CLEAR();
}

/*
 * Writes to tx buffer, and disables ack for this packet
 */
void nrfWriteTxPayloadNoAck(char * data, int len){
    NRF_CSN_SET();
    NRF_SPI_TRANSFER(NRF_SPI_R_RX_PAYLOAD);
    unsigned int i;
    for(i=0;i<len;i++){
       data[i] = NRF_SPI_TRANSFER(TRASH);
    }
    NRF_CSN_CLEAR();

}

void nrfPrintStringConstant(const char * data){
    NRF_CSN_SET();
    while(*data != '\0'){
        NRF_SPI_TRANSFER(*data);
        data++;
    }
     NRF_CSN_CLEAR();
}

void nrfPrintString(char * data){
    NRF_CSN_SET();
    while(*data != '\0'){
        NRF_SPI_TRANSFER(*data);
        data++;
    }
    NRF_CSN_CLEAR();
}

/**
 * Set's chip into rx mode (doesn't activate)
 */
void nrfSetRxMode(){
    nrfSetBit(NRF_CONFIG, NRF_CONFIG_PRIM_RX);
}
void nrfSetTxMode(){
    nrfClearBit(NRF_CONFIG, NRF_CONFIG_PRIM_RX);
}

/**
 * Puts the chip into rx/tx mode
 */
void nrfActivate(unsigned char rx_active){
    if(rx_active){
        nrfSetBit(NRF_CONFIG, NRF_CONFIG_PRIM_RX);
    }else{
         nrfClearBit(NRF_CONFIG, NRF_CONFIG_PRIM_RX);
    }
    NRF_CE_SET();
    delay_us(10);     //wait for 130 microseconds for chip to enter rx/tx mode

}

/**
 * Puts the chip into tx mode, and then clears CEm,
 * allowing the chip to return to
 *
 */
void nrfSend(){
    nrfClearBit(NRF_CONFIG,NRF_CONFIG_PRIM_RX);//set's chip as transmitter
    NRF_CE_SET(); //activate
    delay_us(11);  //allow activation to complete
    
}

void nrfSetRxAddressP0(unsigned char address[5]){
    nrfWriteMultibyteReg(NRF_RX_ADDR_P0,&address[0],5);

}

void nrfSetRxAddressP1(unsigned char address[5]){
    nrfWriteMultibyteReg(NRF_RX_ADDR_P0,&address[0],5);

}

/**
 * set's up the nrf with some basic settings
 */
void nrfSetup(){
   
}

//abstract various spi devices with seperate FIFOs
//NRF_SPI_TRANSFER(NRF,byte)
