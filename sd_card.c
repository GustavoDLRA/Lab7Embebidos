#include "sd_card.h"
#include "spi.h"
#include "myprintf.h"

void initCycles(void) {
	uint32_t i;
	REG_PORT_OUTSET0 = PORT_PA18;
	for(i=0;i<76;i++)
		spiSend(0xFF);
}

uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff ) {
	uint8_t temp = 0xFF;  
	uint32_t i;
	uint8_t temp_cmd = send_buff[0];
	
	REG_PORT_OUTCLR0 = PORT_PA18; // CS
	for(i=0; i< bc; i++) {
		temp = spiSend(*(send_buff++));
		myprintf(" %x", temp);
	}

	myprintf("\n");
	
	switch(temp_cmd) {
		case kCMD00 :
			for(i=0; i<LENGTH_R1; i++) {
				temp = spiSend(0xFF);
				*(receive_buff++) = temp;
				myprintf(" %x", temp);
			}
			break;
		case kCMD08 :
			for(i=0; i<LENGTH_R7; i++) {
				temp = spiSend(0xFF);
				*(receive_buff++) = temp;
				myprintf(" %x", temp);
			}
			break;
		case kCMD41 :
			for(i=0; i<LENGTH_R1-1; i++) {
				temp = spiSend(0xFF);
				*(receive_buff++) = temp;
				myprintf(" %x", temp);
			}
			spiSend(0xFF);
			break;
		case kCMD55 :
			for(i=0; i<LENGTH_R1; i++) {
				temp = spiSend(0xFF);
				*(receive_buff++) = temp;
				myprintf(" %x", temp);
			}
			break;
	default :
	 	myprintf("\n Error in CMD");
	}
	myprintf("\n");
	
	REG_PORT_OUTSET0 = PORT_PA18; // CS
	return(temp);
}

void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs ) {
    uint8_t temp = 0xFF;
    uint32_t i; 

    REG_PORT_OUTCLR0 = PORT_PA18;
    myprintf("\n\n"); 

    // Send command
    temp = send_buff[0];
    temp = spiSend(temp);
    myprintf(" %x", temp);

    // Send argument
    for (i=0; i<4; i++) {
        temp = ((uint8_t*) &lba)[3-i];
        temp = spiSend(temp);
        myprintf(" %x", temp); 
    }

    // Send CRC
    temp = send_buff[5];
    temp = spiSend(temp);
    myprintf(" %x", temp); 

    // Reading to find the beginning of the sector  
    temp = spiSend(0xFF);
    while(temp != 0xFE){
       temp = spiSend(0xFF);
       myprintf(" %x", temp);
    } 

    // Receiving the memory sector/block 

    myprintf("\n\n");
    for(i=0; i< bs; i++) {
        while(SERCOM1->SPI.INTFLAG.bit.DRE == 0); // Wait until buffer is empty
        SERCOM1->SPI.DATA.reg = 0xFF;             // Transmit 0xFF
        while(SERCOM1->SPI.INTFLAG.bit.TXC == 0); // Wait until there is no data to transmit
        while(SERCOM1->SPI.INTFLAG.bit.RXC == 0); // Wait until a data is received
        temp = SERCOM1->SPI.DATA.reg;             // Read data
        *(receive_buff++) = temp;                 // Pass data to receive_buff
        myprintf(" %02x,", i, temp);                    // And print it... because why not?
    }
    REG_PORT_OUTSET0 = PORT_PA18;
    myprintf("\n\n");
    
}
