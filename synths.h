

#define FSMAGIC 0x0250FF0F //quieter than ? 0x0405FF08;
//try ff50ff08
//0xFF056408 from code example
 //timekeep-1 startwait5 standbywait100 rstbwait8
 //2 8 FF 8
// 2 16 FF 08
#define CLKDIVMAGIC ((8)<<9) //7 and 8 2<<7
#define BCKMAGIC 6<<6 //6 7
#define CLKMAGIC 6 //4
#define ADC1_PATT (0x6C<<24)
#define ADC2_PATT (0x0D<<24)


#include "sketch.h"

#define delaysiz (1<<16)
uint8_t *delaybuffa;
uint8_t * delaybuffb;

uint8_t * delaybuffd;
uint8_t * delaybuffc;

uint8_t *delptr=delaybuffa; 
uint8_t *belptr=delaybuffc; 


static int delayptr;
static int delayskp;
static int lastskp;
uint8_t dell;

int adc_read;
int akkuval;
bool butt;
bool buttest;
int buttflip;
  int readr;

int ppread;
int gyo;
int forsh;
int rdr;
int wtr;
int bullshit;
int tima;
int timahi;
int preset;
void (*presets[2]) ();


void IRAM_ATTR jigHandler() {
  int buttnow = (GPIO_IN1_REG[0]&0x1);
  
     REG(GPIO_STATUS_W1TC_REG)[0]=0xFFFFFFFF;
 REG(GPIO_STATUS1_W1TC_REG)[0]=0xFFFFFFFF;
 
 if (buttnow) butt = !butt;
REG(TIMG0_T0UPDATE_REG)[0]=BIT(1);

 tima=REG(TIMG0_T0LO_REG)[0];
 timahi=REG(TIMG0_T0HI_REG)[0];
 //REG(TIMG0_T0CONFIG_REG)[0]=(1<<15)|BIT(30)|BIT(31);
REG(TIMG0_T0LOAD_REG)[0]=BIT(1);

 if (!buttnow) 
  if (tima<0x40000){
   preset++;
   preset = preset %2;
   attachInterrupt(2,presets[preset],FALLING);
   //attachInterrupt(2,presets[0],FALLING);


  }
}




//ptr between 0 and 1ffff
int dellius(int ptr, int val, bool but) {
 int zut,biz,forsh;
 if (ptr&0x10000) 
  delptr = delaybuffa;
 else 
  delptr = delaybuffb;
 ptr = ptr&0xFFFF;
 ptr = ptr * 3;
 biz = ptr&1;
 forsh = biz << 2;
 zut = delptr[(ptr>>1)+biz]<<4;
 zut |= (delptr[(ptr>>1)+1-biz]&(0xF<<(forsh)))>>(forsh);
 if (!but) {
  delptr[(ptr>>1)+biz]=(uint8_t)(val>>4);
  delptr[(ptr>>1)+1-biz]&=(uint8_t)(0xF<<(4-forsh));
  delptr[(ptr>>1)+1-biz]|=(uint8_t)(val&(0xF<<forsh));
 }
 return zut;
}


int myNumbers[] = {32000, 31578, 22444, 25111};
//you need to make a table that is 0,3000,5578
int myPlacers[] = {0, 0, 0, 0};


//int myNumbers[] = {12000, 11578, 14444, 15111,8900, 10278, 12004, 12111};
//you need to make a table that is 0,3000,5578
//int myPlacers[] = {0, 0, 0, 0,0,0,0,0};

int tapsz=sizeof(myPlacers)>>2;


int hipass;
void IRAM_ATTR vinegarHandler() {
 REG(GPIO_STATUS_W1TC_REG)[0]=0xFFFFFFFF;
 REG(GPIO_STATUS1_W1TC_REG)[0]=0xFFFFFFFF;
 REG(SPI3_W8_REG)[0]=(0x9000|ppread)<<16;
 REG(SPI3_CMD_REG)[0]=BIT(18);
 gyo=REG(SPI3_W0_REG)[0];
 gyo =gyo>>16;
 gyo&=0xFFF;
 
 //gyo=0xFFF-gyo;
//delpta, delpte, forsha, forshe
//adc, dac
//num
 bullshit = 2;
 ppread =0;
 for (int i=0; i<tapsz; i++) 
  ppread+=dellius((myPlacers[i]<<2)+i,gyo,butt);
  //there was a weird multitap mistake
  //ppread+=dellius((myPlacers[i]<<2)+(i<<2),gyo,butt);
 hipass=((hipass*255)+(ppread-2048))>>8;
 //ppread = ppread-hipass;
 ppread = ppread>>2;
 
  if (GPIO_IN1_REG[0]&0x8)
   for (int i=0; i<tapsz; i++)  //sizeof(myPlacers)
    myPlacers[i]++;
  else 
   for (int i=0; i<tapsz; i++) 
    myPlacers[i]--;
  for (int i=0; i<tapsz; i++) {
   myPlacers[i] %= myNumbers[i];
   
   if (myPlacers[i]<0) myPlacers[i] += myNumbers[i];
  }
  //delayptr=delayptr&0x1FFFF;//
  //delayptr=delayptr&(0x1FFFF>>(adc_read>>6));

  if (GPIO_IN1_REG[0]&0x4)  {
   if (lastskp==0) delayskp = delayptr;
   lastskp = 1;
  } else {
   if (lastskp) delayptr=delayskp;
   lastskp = 0;
  } 


   REG(I2S_CONF_REG)[0] &= ~(BIT(5)); 
 volatile uint32_t *rr = REG(I2S_FIFO_RD_REG);
 adc_read = rr[0]&0x7FF;
 adc_read=((adc_read>>3));//-32);
 if (adc_read<0)adc_read=0;
 REG(ESP32_RTCIO_PAD_DAC1)[0] =  BIT(10) | BIT(17) | BIT(18) |  ((adc_read&0xFF)<<19);
 //REG(ESP32_RTCIO_PAD_DAC1)[0] = BIT(10) | BIT(17) | BIT(18) |  ((delayptr&0xFF)<<19);
 //REG(ESP32_RTCIO_PAD_DAC1)[0] = BIT(10) | BIT(17) | BIT(18) |  ((rand()&0xFF)<<19);
    
 REG(I2S_INT_CLR_REG)[0]=0xFFFFFFFF;
 REG(I2S_CONF_REG)[0] |= (BIT(5)); //start rx
 GPIO_OUT_REG[0]=(uint32_t)(myPlacers[0]+myPlacers[1]+myPlacers[2]+myPlacers[3]<<12);
 if (butt) GPIO_OUT_REG[3]=2;
 else GPIO_OUT_REG[3]=0;  
}



void IRAM_ATTR whigHandler() {
 REG(GPIO_STATUS_W1TC_REG)[0]=0xFFFFFFFF;
 REG(GPIO_STATUS1_W1TC_REG)[0]=0xFFFFFFFF;
 REG(SPI3_W8_REG)[0]=(0x9000|ppread)<<16;
 REG(SPI3_CMD_REG)[0]=BIT(18);
 gyo=REG(SPI3_W0_REG)[0];
 gyo =gyo>>16;
//delpta, delpte, forsha, forshe
//adc, dac
//num
 bullshit = 2;
 ppread=dellius(delayptr,gyo,butt);
 
  if (GPIO_IN1_REG[0]&0x8) delayptr++;
  else delayptr--; 
  delayptr=delayptr&0x1FFFF;//
  //delayptr=delayptr&(0x1FFFF>>(adc_read>>6));

  if (GPIO_IN1_REG[0]&0x4)  {
   if (lastskp==0) delayskp = delayptr;
   lastskp = 1;
  } else {
   if (lastskp) delayptr=delayskp;
   lastskp = 0;
  } 


   REG(I2S_CONF_REG)[0] &= ~(BIT(5)); 
 volatile uint32_t *rr = REG(I2S_FIFO_RD_REG);
 adc_read = rr[0]&0x7FF;
 adc_read=((adc_read>>3));//-32);
 if (adc_read<0)adc_read=0;
 REG(ESP32_RTCIO_PAD_DAC1)[0] =  BIT(10) | BIT(17) | BIT(18) |  ((adc_read&0xFF)<<19);
 //REG(ESP32_RTCIO_PAD_DAC1)[0] = BIT(10) | BIT(17) | BIT(18) |  ((delayptr&0xFF)<<19);
 //REG(ESP32_RTCIO_PAD_DAC1)[0] = BIT(10) | BIT(17) | BIT(18) |  ((rand()&0xFF)<<19);
    
 REG(I2S_INT_CLR_REG)[0]=0xFFFFFFFF;
 REG(I2S_CONF_REG)[0] |= (BIT(5)); //start rx
 GPIO_OUT_REG[0]=(uint32_t)(delayptr<<12);
 if (butt) GPIO_OUT_REG[3]=2;
 else GPIO_OUT_REG[3]=0;  
}


void IRAM_ATTR pigHandler() {
 REG(GPIO_STATUS_W1TC_REG)[0]=0xFFFFFFFF;
 REG(GPIO_STATUS1_W1TC_REG)[0]=0xFFFFFFFF;
 REG(SPI3_W8_REG)[0]=(0x9000|ppread)<<16;
 REG(SPI3_CMD_REG)[0]=BIT(18);
 gyo=REG(SPI3_W0_REG)[0];
 gyo =gyo>>16;
//delpta, delpte, forsha, forshe
//adc, dac
//num
 bullshit = 1;
 
  if (delayptr&0x10000) {
    delptr = delaybuffa;
    belptr = delaybuffc;
    forsh=4;
   }
   else {
    delptr = delaybuffb;
    belptr = delaybuffd;
    forsh=0;
   }
   ppread=((delptr[delayptr&0xFFFF])<<4);    
   ppread |= ((belptr[delayptr>>1&0x7FFF])&(0xF<<forsh)>>forsh);
 if (!butt) {
  delptr[delayptr&0xFFFF]=(uint8_t)(gyo>>4);
  belptr[delayptr>>1&0x7FFF]&=(uint8_t)(gyo&0xF<<(4-forsh));
  belptr[delayptr>>1&0x7FFF]|=(uint8_t)(gyo&0xF<<forsh);
  
 }
  if (GPIO_IN1_REG[0]&0x8) delayptr++;
  else delayptr--; 
  delayptr=delayptr&0x1FFFF;//
  //delayptr=delayptr&(0x1FFFF>>(adc_read>>6));

  if (GPIO_IN1_REG[0]&0x4)  {
   if (lastskp==0) delayskp = delayptr;
   lastskp = 1;
  } else {
   if (lastskp) delayptr=delayskp;
   lastskp = 0;
  } 


   REG(I2S_CONF_REG)[0] &= ~(BIT(5)); 
 volatile uint32_t *rr = REG(I2S_FIFO_RD_REG);
 adc_read = rr[0]&0x7FF;
 adc_read=((adc_read>>3));//-32);
 if (adc_read<0)adc_read=0;
 REG(ESP32_RTCIO_PAD_DAC1)[0] =  BIT(10) | BIT(17) | BIT(18) |  ((adc_read&0xFF)<<19);
 //REG(ESP32_RTCIO_PAD_DAC1)[0] = BIT(10) | BIT(17) | BIT(18) |  ((delayptr&0xFF)<<19);
 //REG(ESP32_RTCIO_PAD_DAC1)[0] = BIT(10) | BIT(17) | BIT(18) |  ((rand()&0xFF)<<19);
    
 REG(I2S_INT_CLR_REG)[0]=0xFFFFFFFF;
 REG(I2S_CONF_REG)[0] |= (BIT(5)); //start rx
 GPIO_OUT_REG[0]=(uint32_t)(delayptr<<12);
 if (butt) GPIO_OUT_REG[3]=2;
 else GPIO_OUT_REG[3]=0;  




}


void setup() { 
 presets[0]=whigHandler;
 presets[1]=vinegarHandler;
 delaybuffa=(uint8_t*)malloc(delaysiz+(delaysiz>>1));
 delptr=delaybuffa;
 delayptr=0;
 delaybuffb=(uint8_t*)malloc(delaysiz+(delaysiz>>1));

 //delaybuffd=(uint8_t*)malloc(delaysiz>>1);
 
 //delaybuffc=(uint8_t*)malloc(delaysiz>>1);

// delaybuffc=(uint8_t*)malloc(delaysiz);
printf("yodel %08x,%08x,%08x,%08x\n",delaybuffa,delaybuffb,delaybuffa,delaybuffb);
 //printf("yodel %08x,%08x,%08x,%08x\n",delaybuffa,delaybuffb,delaybuffc,delaybuffd);
 //esp_task_wdt_init(30, false);
  //to be fixed

esp_task_wdt_init(30, false);
     // REG(SENS_SAR_ATTEN2_REG)[0]=1;//0x1;
    //REG(ESP32_SENS_SAR_MEAS_START2)[0]=BIT(18)|BIT(31)|BIT(19); //pin g4
    
     //REG(ESP32_SENS_SAR_MEAS_START2)[0]=BIT(18)|BIT(31)|BIT(19)|BIT(17);


 
     
  REG(ESP32_SENS_SAR_DAC_CTRL1)[0] = 0x0; 
  REG(ESP32_SENS_SAR_DAC_CTRL2)[0] = 0x0; 
  //initiate DIG
      initDIG();
      //initRTC();
  //function 2 on the 12 block
  REG(IO_MUX_GPIO12ISH_REG)[0]=BIT(13); //sdi2 q MISO
  REG(IO_MUX_GPIO12ISH_REG)[1]=BIT(13); //d MOSI
  REG(IO_MUX_GPIO12ISH_REG)[2]=BIT(13); //clk
  REG(IO_MUX_GPIO12ISH_REG)[3]=BIT(13); //cs0

//perip clock bit 16 is spi3, 13 is timer0
  CHANGOR(DPORT_PERIP_CLK_EN_REG,BIT(16)|BIT(13))
  CHANGNOR(DPORT_PERIP_RST_EN_REG,BIT(16)|BIT(13))


   REG(TIMG0_T0CONFIG_REG)[0]=(1<<18)|BIT(30)|BIT(31);
   
  
  REG(IO_MUX_GPIO5_REG)[0]=BIT(12); //sdi3 cs0
  REG(IO_MUX_GPIO18_REG)[0]=BIT(12); //sdi3 clk
  REG(IO_MUX_GPIO19_REG)[0]=BIT(12)|BIT(9); //sdi3 q MISO
  REG(IO_MUX_GPIO23_REG)[0]=BIT(12); //sdi3 d MOSI
  REG(SPI3_MOSI_DLEN_REG)[0]=15;
  REG(SPI3_MISO_DLEN_REG)[0]=15;
  REG(SPI3_USER_REG)[0]=BIT(25)|BIT(0)|BIT(27)|BIT(28)|BIT(7)|BIT(6)|BIT(5)|BIT(11)|BIT(10);
    //USR_MOSI, MISO_HIGHPART, and DOUTDIN
  REG(SPI3_PIN_REG)[0]=BIT(29);
  //REG(SPI3_CTRL2_REG)[0]=BIT(17);


  REG(SPI3_CLOCK_REG)[0]=(1 <<18)|(3<<12)|(1<<6)|3;

  #define SPINNER 500000
  #define SPIRTER(d) \
  REG(SPI3_W8_REG)[0]=(d)<<16; \ 
  REG(SPI3_CMD_REG)[0]=BIT(18); \
  spin(SPINNER);
  spin(SPINNER*5);

  //do a few nops firszt
  printf("yoz\n"); 
  SPIRTER(0); //adc_seq,9rep,1chan0
  SPIRTER(0); //adc_seq,9rep,1chan0
  
  //SPIRTER(0b0111110110101100); //sw_reset
  SPIRTER(0x1201); //adc_seq,9rep,1chan0
  SPIRTER(0x1800); //gen_ctrl_reg
  SPIRTER(0x2001); //adc_config,io0adc0
  SPIRTER(0x2802); //dac_config,io1dac1
  SPIRTER(0x5a00); //pd_ref_ctrl,9vref
  
  printf("yoz\n"); 
  //SPIRTER(0b0111110110101100); //sw_reset
  SPIRTER(0x1201); //adc_seq,9rep,1chan0
  SPIRTER(0x1800); //gen_ctrl_reg
  SPIRTER(0x2001); //adc_config,io0adc0
  SPIRTER(0x2802); //dac_config,io1dac1
  SPIRTER(0x5a00); //pd_ref_ctrl,9vref
  

  //straight out
  //LED//  
  GPIO_FUNC_OUT_SEL_CFG_REG[33]=256;
  
  GPIO_FUNC_OUT_SEL_CFG_REG[12]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[13]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[14]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[15]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[16]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[17]=256;
  //GPIO_FUNC_OUT_SEL_CFG_REG[18]=256;
  //GPIO_FUNC_OUT_SEL_CFG_REG[19]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[21]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[22]=256;
  //GPIO_FUNC_OUT_SEL_CFG_REG[23]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[26]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[27]=256;
  REG(GPIO_ENABLE_REG)[0]=BIT(12)|BIT(13)
  |BIT(14)|BIT(15)|BIT(16)|BIT(17)
  |BIT(21)|BIT(22)|BIT(26)|BIT(27); //ouit freaqs  
  REG(GPIO_ENABLE_REG)[3]=2; //output enable 33
  REG(IO_MUX_GPIO32_REG)[0]=BIT(9)|BIT(8); //input enable
  REG(IO_MUX_GPIO34_REG)[1]=BIT(9)|BIT(8); //input enable
  REG(IO_MUX_GPIO34_REG)[0]=BIT(9)|BIT(8); //input enable
 REG(IO_MUX_GPIO2_REG)[0]=BIT(9)|BIT(8); //input enable
 
 attachInterrupt(2,whigHandler,FALLING);
 attachInterrupt(32,jigHandler,CHANGE);
}
void loop() {} 

void sloop() {

     volatile uint32_t *rr = REG(ESP32_SENS_SAR_MEAS_START2);
         uint32_t rdrr = rr[0];
    if (rdrr&(1<<16)) { 
      rdr=(rdrr>>3&0xFFF)-220;
      if(rdr<0)rdr=0;
      REG(SENS_SAR_ATTEN2_REG)[0]=1;//0x1;
      REG(ESP32_SENS_SAR_MEAS_START2)[0]=BIT(18)|BIT(31)|BIT(19); //pin g4    
       REG(ESP32_SENS_SAR_MEAS_START2)[0]=BIT(18)|BIT(31)|BIT(19)|BIT(17);
REG(ESP32_RTCIO_PAD_DAC1)[0] =  BIT(10) | BIT(17) | BIT(18) |  (wtr)<<19;
     }
}



void yloop() {
  int ryo;
  unsigned int buttress=0;
 // return;
  printf("yo");
  for (;;) {
    ryo++;
    if (ryo>100000) ryo = 0;  
    if (ryo==0) {
      
     //int gyo;
     
   //prr("I2S_INT_RAW_REG",I2S_FIFO_RD_REG); 



     // printf("\n-----%x-------%x\n",buttress%5,buttress--); 
      //if (buttress<-10) buttress=0;
   
   //printf("\n-----%x-------%x\n",(int)bullshit,(int)tima); 
     
     //printf("\n-----%d-------%d\n",(int)REG(SPI2_USER_REG)[0],REG(SPI2_MOSI_DLEN_REG)[0]); 
     //printf("\n-----%x-------%x\n",REG(SPI3_W8_REG)[0],REG(SPI3_W0_REG)[0]); 

     
    }
 } 
}


  
