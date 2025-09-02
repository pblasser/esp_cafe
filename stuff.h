#include "setup.h"

#define ENNEAGRAM simpleHelpers
#define SETUPPERS initDEL();
#define BUTTONEST REG(GPIO_IN1_REG)[0]&0x1
#define CLICKETTE(c) attachInterrupt(32,c,CHANGE);
#define DOUBLECLK CLICKETTE(doubleclicker);
#define TRIPLECLK
#define PRESETTER(p) attachInterrupt(2,p,FALLING);
#define LAMPLIGHT \
 REG(GPIO_OUT_REG)[3]=((lamp?1:0)<<1);
#define LAMPAFLIP lamp=!lamp;LAMPLIGHT
#define EARTHREAD (REG(I2S_FIFO_RD_REG)[0]&0x7FF)>>3
#define ASHWRITER(a) \
 REG(ESP32_RTCIO_PAD_DAC1)[0]= \
 BIT(10)|BIT(17)|BIT(18)|((a&0xFF)<<19);
#define INTABRUPT \
 REG(GPIO_STATUS_W1TC_REG)[0]=0xFFFFFFFF; \
 REG(GPIO_STATUS1_W1TC_REG)[0]=0xFFFFFFFF;
#define SPIWRITER(d) \
 REG(SPI3_W8_REG)[0]=(d)<<16; \
 REG(SPI3_CMD_REG)[0]=BIT(18);
#define DACWRITER(p) SPIWRITER(0x9000|p)
#define ADCREADER ((REG(SPI3_W0_REG)[0])>>16)&0xFFF;
#define I2S_START
#define I2SFINISH
#define YELLOWERS(b) \
 REG(GPIO_OUT_REG)[0]=((uint32_t)(b)<<12);
#define FLIPPERAT REG(GPIO_IN1_REG)[0]&0x8
#define SKIPPERAT REG(GPIO_IN1_REG)[0]&0x4
#define DELAYSIZE (1<<17)
#define FILLNOISE \
 for(int i=0;i<DELAYSIZE;i++) dellius(i,rand(),false);

bool lamp;
int tima;
int timahi;
int preset;
void (*presets[2]) ();

void IRAM_ATTR doubleclicker() {
 INTABRUPT
 int buttnow = BUTTONEST;
 if (buttnow) lamp=!lamp;
 LAMPLIGHT
 
 REG(TIMG0_T0UPDATE_REG)[0]=BIT(1);
 tima=REG(TIMG0_T0LO_REG)[0];
 timahi=REG(TIMG0_T0HI_REG)[0];
 //REG(TIMG0_T0CONFIG_REG)[0]=(1<<15)|BIT(30)|BIT(31);
 REG(TIMG0_T0LOAD_REG)[0]=BIT(1);

 if (buttnow==0) 
  if (tima<0x40000){
   preset++;
   preset = preset %2;
   PRESETTER(presets[preset])
   //attachInterrupt(2,presets[0],FALLING);
  }
}

uint8_t *delaybuffa;
uint8_t *delaybuffb;
uint8_t *delptr; 
static int delayptr;
static int delayskp;
static int lastskp;
int adc_read;
int gyo;
int ppread; //persistent_red

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

void initDEL() {
 delaybuffa=(uint8_t*)malloc((DELAYSIZE>>2)+(DELAYSIZE>>1));
 delaybuffb=(uint8_t*)malloc((DELAYSIZE>>2)+(DELAYSIZE>>1));
 delptr=delaybuffa;
 delayptr=0;

 esp_task_wdt_init(30, false);
 
 REG(ESP32_SENS_SAR_DAC_CTRL1)[0] = 0x0; 
 REG(ESP32_SENS_SAR_DAC_CTRL2)[0] = 0x0; 

 initDIG();
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
 #define SPRINTER(a) SPIWRITER(a); spin(SPINNER);
 
 spin(SPINNER*5);
 SPRINTER(0); 
 SPRINTER(0); 
  
  //SPIRTER(0b0111110110101100); //sw_reset
  SPRINTER(0x1201); //adc_seq,9rep,1chan0
  SPRINTER(0x1800); //gen_ctrl_reg
  SPRINTER(0x2001); //adc_config,io0adc0
  SPRINTER(0x2802); //dac_config,io1dac1
  SPRINTER(0x5a00); //pd_ref_ctrl,9vref
  
  //SPIRTER(0b0111110110101100); //sw_reset
  SPRINTER(0x1201); //adc_seq,9rep,1chan0
  SPRINTER(0x1800); //gen_ctrl_reg
  SPRINTER(0x2001); //adc_config,io0adc0
  SPRINTER(0x2802); //dac_config,io1dac1
  SPRINTER(0x5a00); //pd_ref_ctrl,9vref
  

  //straight out
  //LED//
  #define GPIO_FUNC_OUT_SEL_CFG_REG REG(0X3ff44530)   
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
 
 }
