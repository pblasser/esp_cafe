#include "mdk.h"
#include <esp_task_wdt.h>

static uint32_t dmall[3];









void initRTC() {
  esp_task_wdt_init(30, false);
//REG(DPORT_WIFI_CLK_EN_REG)[0]=0;
  REG(SENS_SAR_READ_CTRL2_REG)[0] |= BIT(29); //inv
  REG(SENS_SAR_MEAS_START2_REG)[0] |= BIT(31);
  REG(SENS_SAR_MEAS_START2_REG)[0] |= BIT(18);
  REG(SENS_SAR_ATTEN2_REG)[0] = 0x1;//4 IO_MUX and GPIO Matrix (GPIO, IO_MUX)
  //REG(SENS_SAR_MEAS_CTRL_REG)[0] &= ~((uint32_t)0xFFFF<<0); //clear fsm
  //REG(SENS_SAR_MEAS_WAIT1_REG)[0] = 0x00010001;
  //REG(SENS_SAR_MEAS_WAIT2_REG)[0] |= (2<<16);//powerdiwb forx xpdamp0
  //REG(SENS_SAR_MEAS_WAIT2_REG)[0] &= ~((uint32_t)0xFFFF);
  //REG(SENS_SAR_MEAS_WAIT2_REG)[0] |= 0x2;//|BIT(17)|BIT(19);
  //REG(SENS_SAR_MEAS_WAIT2_REG)[0] |= BIT(18)|BIT(19); //force power on to xpd sar  
  //REG(SENS_SAR_TOUCH_ENABLE_REG)[0] = 0; //touch pads off
  REG(ESP32_RTCIO_ADC_PAD)[0] = BIT(28)|BIT(22)|BIT(21)|BIT(18);
//28 is adc2 mux rtc, 22 fun sel 21, 18 ie
//for sar2 that is
 // REG(ESP32_RTCIO_ADC_PAD)[0] = BIT(28)|BIT(18)|
  //    BIT(29)|BIT(27)|BIT(26)|BIT(23);
    REG(ESP32_SENS_SAR_MEAS_START2)[0]=BIT(18)|BIT(31)|BIT(19); //pin g4
    REG(ESP32_SENS_SAR_MEAS_START2)[0]=BIT(18)|BIT(31)|BIT(19)|BIT(17);
}

























void initDIG() {
  //CHANG(SENS_SAR_ATTEN1_REG,0x2<<12)
  //CHANG(SENS_SAR_ATTEN2_REG,0x2)
  CHANG(DPORT_WIFI_CLK_EN_REG,0)
  //SPI3 CLOCK
  CHANGOR(DPORT_PERIP_CLK_EN_REG,BIT(4))
  CHANGNOR(DPORT_PERIP_RST_EN_REG,BIT(4))
  //ADC POWER ALWAYS ON
  CHANGNO(SENS_SAR_MEAS_CTRL_REG,(uint32_t)0xFFFF)
  
  CHANG(SENS_SAR_MEAS_CTRL_REG,(uint32_t)0)
  CHANG(SENS_SAR_MEAS_CTRL2_REG,(uint32_t)0)
//LNA low noise amp example
  //CHANG(SENS_SAR_MEAS_CTRL_REG,(uint32_t)0xFF3F038F)
CHANG(SENS_SAR_MEAS_CTRL_REG,(uint32_t)0xFF07338F) //default
  //REG(SENS_SAR_MEAS_WAIT1_REG)[0] = 0xFFFFFFFF;
  CHANG(SENS_SAR_MEAS_WAIT2_REG,BIT(18)|BIT(19)|BIT(17)|BIT(16)|0xFFF)

  CHANG(I2S_INT_ENA_REG,0) //disable interrupt
  CHANGNO(I2S_INT_CLR_REG,0)
  CHANG(I2S_CONF_REG,0)
  CHANGNOR(I2S_CONF_REG,BIT(1))//rx reset
  CHANGOR(I2S_CONF_REG,BIT(17)|BIT(9)) //msb right first
  
  CHANGNOR(I2S_CONF_REG,BIT(3)) //rx fifo reset
  CHANGOR(I2S_CONF1_REG,BIT(7))//PCM bypass
  
  //enable DMA
  CHANG(I2S_LC_CONF_REG,0)
  CHANGNOR(I2S_LC_CONF_REG,BIT(2))//ahb fifo rst
  CHANGNOR(I2S_LC_CONF_REG,BIT(3))//ahb reset
  CHANGNOR(I2S_LC_CONF_REG,BIT(0))//in rst
  CHANGOR(I2S_LC_CONF_REG,BIT(10))//burst inlink

  CHANG(I2S_CONF2_REG,0);//LCD enable
  CHANGOR(I2S_CONF2_REG,BIT(5));//LCD enable
  
  CHANG(I2S_FIFO_CONF_REG,BIT(12)|BIT(5)|BIT(20))
  CHANG(I2S_FIFO_CONF_REG,BIT(5)|BIT(20))
  CHANG(I2S_FIFO_CONF_REG,BIT(20))
  //NODMA
  //bit 16 is single channel|BIT(17)) is 32bit
  //20forcemod 16rxmod 12dmaconnect 5rxdatanum32
  CHANG(I2S_CONF_CHAN_REG,BIT(3)) //3singlechanrx
  CHANG(I2S_PDM_CONF_REG,0)
  CHANG(I2S_CLKM_CONF_REG,BIT(21)|CLKMAGIC)//clockenable
  //freq 2 is bad, 
  CHANGOR(I2S_SAMPLE_RATE_CONF_REG,(BCKMAGIC))
  //50
  prr("I2S_INT_RAW_REG",I2S_INT_RAW_REG); 
  
  //adc set i2s data len patterns should be zero 
  //adc set data pattern 
  CHANG(APB_SARADC_SAR1_PATT_TAB1_REG,ADC1_PATT)  
  CHANG(APB_SARADC_SAR2_PATT_TAB1_REG,ADC2_PATT)
  
  //adc set controller DIG
  /////////////////////adc 1 was on but now the force is gone
  //CHANGOR(SENS_SAR_READ_CTRL_REG,BIT(27)|BIT(28))
  //CHANG(SENS_SAR_READ_CTRL_REG,BIT(27)|BIT(28)|BIT(17)|BIT(16)|0xFFFF)//0xFFFF
  //bottom bytes sample cycle and clock div
  
  CHANGOR(SENS_SAR_READ_CTRL2_REG,BIT(28)|BIT(29))
  //CHANG(ESP32_SENS_SAR_MEAS_START1,BIT(31)|BIT(19+6)|BIT(18)) 
  //CHANG(ESP32_SENS_SAR_MEAS_START2,BIT(31)|BIT(19)|BIT(18))
   //seems to not need bitmap
 
 
   #define CTRLJING BIT(26)|(CLKDIVMAGIC)|BIT(6)|BIT(2)|BIT(3)
   
   #define CTRLPATT 0 //BIT(15)|BIT(19)
  #define CTRLJONG BIT(24)|BIT(23)
    //26datatoi2s 25sarsel 9clkdiv4 6clkgated 3double 2sar2mux
    //8 clock div 2   
      CHANG(APB_SARADC_CTRL_REG,CTRLJING|CTRLPATT)


  //REG(APB_SARADC_FSM_REG)[0]=0x0216FF08;
  CHANG(APB_SARADC_FSM_REG,0xFF056408) //from code example
  CHANG(APB_SARADC_FSM_REG,0xFF056408) //from code example
  REG(APB_SARADC_FSM_REG)[0]=FSMAGIC;
 //timekeep-1 startwait5 standbywait100 rstbwait8

  CHANGNOR(APB_SARADC_CTRL_REG,CTRLJONG)
//  CHANGOR(I2S_CLKM_CONF_REG,BIT(21)|4)//clockenable
  CHANGOR(APB_SARADC_CTRL2_REG,BIT(10)|BIT(9)|BIT(0));
   CHANG(APB_SARADC_CTRL2_REG,BIT(10)|BIT(9)|BIT(1)|BIT(0));//trying to limit to 1

   //inverting and not inverting 10 and 9 data to adc ctrl no effect
   //CHANG(APB_SARADC_CTRL2_REG,BIT(1)|BIT(0));//trying to limit to 1
   REG(SENS_SAR_TOUCH_ENABLE_REG)[0] = 0;

#define bufflough 7//3
dmall[0]=0xC0|BIT(bufflough+12)|BIT(bufflough);
  //owner dma, eof, 128, 128
  //uint16_t*buff=&dmabuff[0];
  //dmall[1]=(uint32_t)buff;
  dmall[2]=0;
  //uint8_t*duff=&delaybuff[0];
  uint32_t*muff=&dmall[0];
  //printf("iBUFF%08x dBUFF%08x dBmall%08x\n",(int)buff, (int)duff, (int)muff);



  CHANG(APB_SARADC_CTRL_REG,CTRLJONG|CTRLJING|CTRLPATT)
  
  
  CHANG(I2S_RXEOF_NUM_REG,BIT(bufflough-2))
    CHANG(I2S_RXEOF_NUM_REG,1)
  CHANGOR(I2S_IN_LINK_REG,(0xFFFFF&(int)muff))
      //REG(I2S_IN_LINK_REG)[0]|=BIT(30);
  CHANGNOR(I2S_CONF_REG,BIT(1))//rx reset  
  CHANGNOR(I2S_CONF_REG,BIT(3)) //rx fifo reset
    CHANG(APB_SARADC_CTRL_REG,CTRLJING|CTRLPATT)
   //pattern pointer cleared
    CHANGOR(I2S_IN_LINK_REG,BIT(29))
    
    REG(I2S_INT_CLR_REG)[0]=0xFFFF;
  //REG(I2S_CONF_REG)[0]=BIT(5)
 //REG(I2S_CONF_REG)[0]=BIT(5)|BIT(1);
  REG(I2S_CONF_REG)[0]=BIT(17)|BIT(9)|BIT(5); //start rx
  //  REG(I2S_CONF_REG)[0]=BIT(9)|BIT(5); //start rx
}

 
void sset(void) {
  //esp_task_wdt_init(30, false);
  //to be fixed
  
  REG(ESP32_SENS_SAR_DAC_CTRL1)[0] = 0x0; 
  REG(ESP32_SENS_SAR_DAC_CTRL2)[0] = 0x0; 
  initDIG();
      
  //function 2 on the 12 block
  REG(IO_MUX_GPIO12ISH_REG)[0]=BIT(13);
  REG(IO_MUX_GPIO12ISH_REG)[1]=BIT(13);
  REG(IO_MUX_GPIO12ISH_REG)[2]=BIT(13);
  REG(IO_MUX_GPIO12ISH_REG)[3]=BIT(13);
  //straight out
    GPIO_FUNC_OUT_SEL_CFG_REG[5]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[12]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[13]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[14]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[15]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[16]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[17]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[18]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[19]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[21]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[22]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[23]=256;
  GPIO_FUNC_OUT_SEL_CFG_REG[27]=256;
  REG(GPIO_ENABLE_REG)[0]=BIT(5)|BIT(12)|BIT(13)
  |BIT(14)|BIT(15)|BIT(16)|BIT(17)|BIT(18)
  |BIT(19)|BIT(21)|BIT(22)|BIT(23)|BIT(27);
  //36 and 39  
  REG(GPIO_ENABLE_REG)[3]=0;
    REG(IO_MUX_GPIO32_REG)[0]=BIT(9)|BIT(8); //input enable
    REG(IO_MUX_GPIO32_REG)[1]=BIT(9)|BIT(8); //input enable

 // REG(IO_MUX_GPIO36_REG)[0]=BIT(9)|BIT(8); //input enable
 // REG(IO_MUX_GPIO36_REG)[3]=BIT(9)|BIT(8); //input enable
  REG(IO_MUX_GPIO34_REG)[1]=BIT(9)|BIT(8); //input enable
  REG(IO_MUX_GPIO34_REG)[0]=0;


  
  //xtosy(0,digHandler);
  //ets_isr_unmask(1u << 0); 
  //REG(DPORT_PRO_GPIO_INTERRUPT_MAP_REG)[0]=0;
  //REG(GPIO_PIN_REG)[2]=BIT(15)|BIT(8)|BIT(8);
  //7risingedge 8falling) 15prointerrupt 13appinterrup
  REG(IO_MUX_GPIO2_REG)[0]=BIT(9)|BIT(8); //input enable
  //pinMode(2, INPUT_PULLUP);


}  
