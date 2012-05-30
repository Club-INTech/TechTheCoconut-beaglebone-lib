//Copyright 2012 Philippe Tillet
//This file is part of BeagleLib
//BeagleLib is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//BeagleLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along with BeagleLib. If not, see http://www.gnu.org/licenses/.

#include "gpio.h"
#include "register.h"

namespace BeagleLib{

  std::map<GPIO_NAME, GPIO_Descriptor> make_gpio_map(){
    std::map<GPIO_NAME, GPIO_Descriptor> res;
    res[USR0] = (struct GPIO_Descriptor){GPIO1, 1<<22,		   "gpmc_a5"};
    res[USR1] = (struct GPIO_Descriptor){GPIO1, 1<<22,           "gpmc_a6"};
    res[USR2] = (struct GPIO_Descriptor){GPIO1, 1<<23,           "gpmc_a7"};
    res[USR3] = (struct GPIO_Descriptor){GPIO1, 1<<24,           "gpmc_a8"};
    res[GPIO0_7] = (struct GPIO_Descriptor){GPIO0,  1<<7, "ecap0_in_pwm0_out"};
    res[GPIO0_26] = (struct GPIO_Descriptor){GPIO0, 1<<26,         "gpmc_ad10"};
    res[GPIO0_27] = (struct GPIO_Descriptor){GPIO0, 1<<27,         "gpmc_ad11"};
    res[GPIO1_0] = (struct GPIO_Descriptor){GPIO1,     1,          "gpmc_ad0"};
    res[GPIO1_1] = (struct GPIO_Descriptor){GPIO1,  1<<1,          "gpmc_ad1"};
    res[GPIO1_2] = (struct GPIO_Descriptor){GPIO1,  1<<2,          "gpmc_ad2"};
    res[GPIO1_3] = (struct GPIO_Descriptor){GPIO1,  1<<3,          "gpmc_ad3"};
    res[GPIO1_4] = (struct GPIO_Descriptor){GPIO1,  1<<4,          "gpmc_ad4"};
    res[GPIO1_5] = (struct GPIO_Descriptor){GPIO1,  1<<5,          "gpmc_ad5"};
    res[GPIO1_6] = (struct GPIO_Descriptor){GPIO1,  1<<6,          "gpmc_ad6"};
    res[GPIO1_7] = (struct GPIO_Descriptor){GPIO1,  1<<7,          "gpmc_ad7"};
    res[GPIO1_12] = (struct GPIO_Descriptor){GPIO1, 1<<12,         "gpmc_ad12"};
    res[GPIO1_13] = (struct GPIO_Descriptor){GPIO1, 1<<13,         "gpmc_ad13"};
    res[GPIO1_14] = (struct GPIO_Descriptor){GPIO1, 1<<14,         "gpmc_ad14"};
    res[GPIO1_15] = (struct GPIO_Descriptor){GPIO1, 1<<15,         "gpmc_ad15"};
    res[GPIO1_16] = (struct GPIO_Descriptor){GPIO1, 1<<16,           "gpmc_a0"};
    res[GPIO1_17] = (struct GPIO_Descriptor){GPIO1, 1<<17,           "gpmc_a1"};
    res[GPIO1_28] = (struct GPIO_Descriptor){GPIO1, 1<<28,         "gpmc_ben1"};
    res[GPIO1_29] = (struct GPIO_Descriptor){GPIO1, 1<<29,         "gpmc_csn0"};
    res[GPIO1_30] = (struct GPIO_Descriptor){GPIO1, 1<<30,         "gpmc_csn1"};
    res[GPIO1_31] = (struct GPIO_Descriptor){GPIO1, 1<<31,         "gpmc_csn2"};
    res[GPIO2_1] = (struct GPIO_Descriptor){GPIO2,     1,          "gpmc_clk"};
    res[GPIO2_6] = (struct GPIO_Descriptor){GPIO2,  1<<6,         "lcd_data0"};
    res[GPIO2_7] = (struct GPIO_Descriptor){GPIO2,  1<<7,         "lcd_data1"};
    res[GPIO2_8] = (struct GPIO_Descriptor){GPIO2,  1<<8,         "lcd_data2"};
    res[GPIO2_9] = (struct GPIO_Descriptor){GPIO2,  1<<9,         "lcd_data3"};
    res[GPIO2_10] = (struct GPIO_Descriptor){GPIO2, 1<<10,         "lcd_data4"};
    res[GPIO2_11] = (struct GPIO_Descriptor){GPIO2, 1<<11,         "lcd_data5"};
    res[GPIO2_12] = (struct GPIO_Descriptor){GPIO2, 1<<12,         "lcd_data6"};
    res[GPIO2_13] = (struct GPIO_Descriptor){GPIO2, 1<<13,         "lcd_data7"};
    res[GPIO2_22] = (struct GPIO_Descriptor){GPIO2, 1<<22,         "lcd_vsync"};
    res[GPIO2_23] = (struct GPIO_Descriptor){GPIO2, 1<<23,         "lcd_hsync"};
    res[GPIO2_24] = (struct GPIO_Descriptor){GPIO2, 1<<24,          "lcd_pclk"};
    res[GPIO2_25] = (struct GPIO_Descriptor){GPIO2, 1<<25,    "lcd_ac_bias_en"};
    res[GPIO3_19] = (struct GPIO_Descriptor){GPIO3, 1<<19,        "mcasp0_fsr"};
    res[GPIO3_21] = (struct GPIO_Descriptor){GPIO3, 1<<21,     "mcasp0_ahclkx"};
    return res;
  }

  //////////////////////////////////////
  // Input GPIO Pins
  /////////////////////////////////////
  
  GPIO< INPUT >::GPIO(GPIO_NAME name, PULL PullMode) : name_(name)
  {
    unsigned int pull = 0;
    //Configure pull-up
    if(PullMode == PULLUP){
      pull = CONF_PULLUP;
    }
    else if(PullMode == NOPULL){
      pull = CONF_PULL_DISABLE;
    }
    //Configure multiplexer
    pin_mux(GPIOs[name_].pinmux_filename, CONF_GPIO_INPUT|pull);
    //Set pin as input.
    or_reg(GPIOs[name_].mux+GPIO_OE, GPIOs[name_].bit_value);
  }

  DigitalState GPIO< INPUT >::read()
  {
    if(get_reg(GPIOs[name_].mux + GPIO_DATAIN & GPIOs[name_].bit_value)){
      return HIGH;
    }
    return LOW;
  }
  
  
  //////////////////////////////////////
  // Output GPIO Pins
  /////////////////////////////////////

  GPIO< OUTPUT >::GPIO(GPIO_NAME name) : name_(name)
  {
    pin_mux(GPIOs[name_].pinmux_filename, CONF_GPIO_OUTPUT);
    clear_reg(GPIOs[name_].mux+GPIO_OE, GPIOs[name_].bit_value);
  }

  void GPIO <OUTPUT >::write(DigitalState val)
  {
    if(val==HIGH){
      or_reg(GPIOs[name_].mux + GPIO_DATAOUT, GPIOs[name_].bit_value);
    }
    else{
      clear_reg(GPIOs[name_].mux + GPIO_DATAOUT, GPIOs[name_].bit_value);
    }
  }
  
  void GPIO< OUTPUT >::toggle()
  {
    xor_reg(GPIOs[name_].mux + GPIO_DATAOUT, GPIOs[name_].bit_value);
  }

}