/*
 * sd_card.h
 *
 * Created: 25/05/2022 06:55:07 p. m.
 *  Author: gusta
 */ 

#ifndef SD_CARD_H_
#define SD_CARD_H_

#include "sam.h"

void initCycles(void);
uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff );
void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs );

#endif /* SD_CARD_H_ */