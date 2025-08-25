#include "stuff.h"

void IRAM_ATTR coco() {
 INTABRUPT
 DACWRITER(ppread)
 gyo=ADCREADER
 ppread=dellius(delayptr,gyo,lamp);
 if (FLIPPERAT) delayptr++;
 else delayptr--; 
 delayptr=delayptr&0x1FFFF;//
 if (SKIPPERAT)  {
  if (lastskp==0) delayskp = delayptr;
  lastskp = 1;
 } else {
  if (lastskp) delayptr=delayskp;
  lastskp = 0;
 } 
 REG(I2S_CONF_REG)[0] &= ~(BIT(5)); 
 adc_read = EARTHREAD;
 ASHWRITER(adc_read); //rand()
 REG(I2S_INT_CLR_REG)[0]=0xFFFFFFFF;
 REG(I2S_CONF_REG)[0] |= (BIT(5)); //start rx
 YELLOWERS(delayptr)
}


int myNumbers[] = {32000, 31578, 22444, 25111};
//you need to make a table that is 0,3000,5578
int myPlacers[] = {0, 0, 0, 0};


//int myNumbers[] = {12000, 11578, 14444, 15111,8900, 10278, 12004, 12111};
//you need to make a table that is 0,3000,5578
//int myPlacers[] = {0, 0, 0, 0,0,0,0,0};

int tapsz=sizeof(myPlacers)>>2;

void IRAM_ATTR echo() {
 INTABRUPT
 DACWRITER(ppread)
 gyo=ADCREADER
 ppread =0;
 for (int i=0; i<tapsz; i++) 
  ppread+=dellius((myPlacers[i]<<2)+i,gyo,lamp);
 ppread = ppread>>2;
 if (FLIPPERAT)
  for (int i=0; i<tapsz; i++)  //sizeof(myPlacers)
   myPlacers[i]++;
 else 
  for (int i=0; i<tapsz; i++) 
   myPlacers[i]--;
 for (int i=0; i<tapsz; i++) {
  myPlacers[i] %= myNumbers[i];
  if (myPlacers[i]<0) myPlacers[i] += myNumbers[i];
 }
 if (SKIPPERAT)  {} else {} 
 REG(I2S_CONF_REG)[0] &= ~(BIT(5)); 
 adc_read = EARTHREAD;
 ASHWRITER(adc_read); //rand()
 REG(I2S_INT_CLR_REG)[0]=0xFFFFFFFF;
 REG(I2S_CONF_REG)[0] |= (BIT(5)); //start rx
 YELLOWERS(myPlacers[0]+myPlacers[1]+myPlacers[2]+myPlacers[3]);
}
