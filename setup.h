
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_task_wdt.h>

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


#define BIT(x) ((uint32_t) 1U << (x))
#define REG(x) ((volatile uint32_t *) (x))
#define CHANG(reg, val) REG(reg)[0] = (val); 
#define CHANGOR(reg, val) REG(reg)[0] |= (val); 
#define CHANGNO(reg, val) REG(reg)[0] &= ~(uint32_t)(val); 
#define CHANGNOR(reg, val)  CHANGOR(reg, val) \
 CHANGNO(reg, val)

#define SPI3_MISO_DLEN_REG  0x3ff64028
#define SPI3_CMD_REG  0x3ff65000
#define SPI3_CTRL_REG  0x3ff65008
#define SPI3_CTRL2_REG  0x3ff65014
#define SPI3_CLOCK_REG  0x3ff65018
#define SPI3_PIN_REG  0x3ff65034
#define SPI3_W0_REG  0x3ff65080
#define SPI3_W8_REG  0x3ff650a0
#define SPI3_MOSI_DLEN_REG  0x3ff65028
//#define SPI3_MIS0_DLEN_REG  0x3ff6502C
#define SPI3_USER_REG  0x3ff6501C

#define DPORT_PERIP_CLK_EN_REG 0x3FF000C0
#define DPORT_PERIP_RST_EN_REG 0x3FF000C4
#define DPORT_WIFI_CLK_EN_REG 0x3FF000CC
#define DPORT_PRO_GPIO_INTERRUPT_MAP_REG 0x3FF0015C
#define GPIO_PIN_REG 0x3FF44088
//#define IO_MUX_GPIO2_REG 0x3FF49040
#define IO_MUX_GPIO12ISH_REG 0x3FF49030
//#define IO_MUX_GPIO5_REG 0x3FF4906c
//#define IO_MUX_GPIO18_REG 0x3FF49070
//#define IO_MUX_GPIO19_REG 0x3FF49074
//#define IO_MUX_GPIO23_REG 0x3FF4908c
//#define IO_MUX_GPIO16_REG 0x3FF4904C
//#define IO_MUX_GPIO32_REG 0x3FF4901C
//#define IO_MUX_GPIO34_REG 0x3FF49014
//#define IO_MUX_GPIO36_REG 0x3FF49004
//#define GPIO_STATUS_REG 0x3FF44044
//#define GPIO_STATUS_W1TC_REG 0x3FF4404C


#define TIMG0_T0CONFIG_REG 0x3FF5F000
#define TIMG0_T0LO_REG 0x3FF5F004
#define TIMG0_T0HI_REG 0x3FF5F008
#define TIMG0_T0UPDATE_REG 0x3FF5F00C
#define TIMG0_T0LOAD_REG 0x3FF5F020


#define ESP32_SENS_SAR_DAC_CTRL1  0x3ff48898
#define ESP32_SENS_SAR_DAC_CTRL2  0x3ff4889C
#define ESP32_RTCIO_PAD_DAC1 0x3ff48484
#define ESP32_RTCIO_PAD_DAC2 0x3ff48488
#define ESP32_RTCIO_ADC_PAD 0x3ff48480
#define ESP32_SENS_SAR_READ_CTRL2 0x3ff48890
#define ESP32_SENS_SAR_MEAS_START2 0x3ff48894

#define ESP32_SENS_SAR_READ_CTRL1 0x3ff48800
#define ESP32_SENS_SAR_MEAS_START1 0x3ff48854

#define DR_REG_SENS_BASE                        0x3ff48800
#define SENS_SAR_READ_CTRL_REG          (DR_REG_SENS_BASE + 0x0000)
#define SENS_SAR_READ_STATUS1_REG          (DR_REG_SENS_BASE + 0x0004)
#define SENS_SAR_MEAS_WAIT1_REG          (DR_REG_SENS_BASE + 0x0008)
#define SENS_SAR_MEAS_WAIT2_REG          (DR_REG_SENS_BASE + 0x000c)
#define SENS_SAR_MEAS_CTRL_REG          (DR_REG_SENS_BASE + 0x0010)
#define SENS_SAR_READ_STATUS2_REG          (DR_REG_SENS_BASE + 0x0014)
#define SENS_SAR_START_FORCE_REG          (DR_REG_SENS_BASE + 0x002c)
#define SENS_SAR_ATTEN1_REG          (DR_REG_SENS_BASE + 0x0034)
#define SENS_SAR_SLAVE_ADDR1_REG  (DR_REG_SENS_BASE + 0x003c)
#define SENS_SAR_ATTEN2_REG          (DR_REG_SENS_BASE + 0x0038)
#define SENS_SAR_SLAVE_ADDR2_REG  (DR_REG_SENS_BASE + 0x0040)
#define SENS_SAR_MEAS_START1_REG          (DR_REG_SENS_BASE + 0x0054)
#define SENS_SAR_TOUCH_CTRL1_REG          (DR_REG_SENS_BASE + 0x0058)
#define SENS_SAR_TOUCH_CTRL2_REG          (DR_REG_SENS_BASE + 0x0084)
#define SENS_SAR_TOUCH_ENABLE_REG          (DR_REG_SENS_BASE + 0x008c)
#define SENS_SAR_READ_CTRL2_REG          (DR_REG_SENS_BASE + 0x0090)
#define SENS_SAR_MEAS_START2_REG          (DR_REG_SENS_BASE + 0x0094)
#define SENS_SAR_MEAS_CTRL2_REG          (DR_REG_SENS_BASE + 0x0a0)

#define DR_REG_SYSCON_BASE 0x3ff66000 //0x60002600
#define APB_CTRL_SYSCLK_CONF_REG (DR_REG_SYSCON_BASE + 0x0)
#define APB_SARADC_CTRL_REG (DR_REG_SYSCON_BASE + 0x10)
#define APB_SARADC_CTRL2_REG (DR_REG_SYSCON_BASE + 0x14)
#define APB_SARADC_FSM_REG (DR_REG_SYSCON_BASE + 0x18)
#define APB_SARADC_SAR1_PATT_TAB1_REG (DR_REG_SYSCON_BASE + 0x1C)
#define APB_SARADC_SAR2_PATT_TAB1_REG (DR_REG_SYSCON_BASE + 0x2C)

#define RNG_REG 0x3FF75144

#define I2S_FIFO_WR_REG 0x3FF4F000
#define I2S_FIFO_RD_REG 0x3FF4F004 
#define I2S_CONF_REG 0x3FF4F008 
#define I2S_INT_RAW_REG 0x3FF4F00C
#define I2S_INT_ST_REG 0x3FF4F010
#define I2S_INT_ENA_REG 0x3FF4F014
#define I2S_INT_CLR_REG 0x3FF4F018
#define I2S_RXEOF_NUM_REG 0x3FF4F024
#define I2S_CONF_SINGLE_DATA_REG 0x3FF4F028


#define I2S_CONF1_REG 0x3FF4F0A0
#define I2S_PD_CONF_REG 0x3FF4F0A4
#define I2S_CONF2_REG 0x3FF4F0A8
#define I2S_CLKM_CONF_REG 0x3FF4F0AC

#define I2S_SAMPLE_RATE_CONF_REG 0x3FF4F0B0
#define I2S_PDM_CONF_REG 0x3FF4F0B4
#define I2S_STATE_REG 0x3FF4F0BC
#define I2S_TIMING_REG 0x3FF4F01C 
#define I2S_FIFO_CONF_REG 0x3FF4F020 
#define I2S_CONF_SINGLE_DATA_REG 0x3FF4F028
#define I2S_CONF_CHAN_REG 0x3FF4F02c
#define I2S_LC_HUNG_CONF_REG 0x3FF4F074
#define I2S_LC_CONF_REG 0x3FF4F060
#define I2S_LC_STATE0_REG 0x3FF4F06C
#define I2S_LC_STATE1_REG 0x3FF4F070
#define I2S_STATE_REG 0x3FF4F0BC
#define I2S_IN_EOF_DES_ADDR_REG 0x3FF4F03C
#define I2S_INLINK_DSCR_BF0_REG 0x3FF4F04C
#define I2S_INLINK_DSCR_BF1_REG 0x3FF4F050


#define I2S_RXEOF_NUM_REG 0x3FF4F024
#define I2S_IN_LINK_REG 0x3FF4F034
#define I2S_INLINK_DSCR_REG 0x3FF4F048

#define ESP32_DPORT 0x3ff00000





static inline void spin(volatile unsigned long count) {
  while (count--) asm volatile("nop");
}

//#define GPIO_FUNC_OUT_SEL_CFG_REG REG(0X3ff44530)  // Pins 0-39
//#define GPIO_FUNC_IN_SEL_CFG_REG REG(0X3ff44130)   // Pins 0-39
//#define GPIO_OUT_REG REG(0X3ff44004)               // Pins 0-31
//#define GPIO_IN_REG REG(0x3FF4403C)                // Pins 0-31
//#define GPIO_ENABLE_REG REG(0X3ff44020)            // Pins 0-31
//#define GPIO_OUT1_REG REG(0X3ff44010)              // Pins 32-39
//#define GPIO_IN1_REG REG(0X3ff44040)               // Pins 32-39
//#define GPIO_ENABLE1_REG REG(0X3ff4402c)           // Pins 32-39


static uint32_t dmall[3];

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
  //prr("I2S_INT_RAW_REG",I2S_INT_RAW_REG); 
  
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
