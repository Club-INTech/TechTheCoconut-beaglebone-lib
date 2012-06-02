//Copyright 2012 Philippe Tillet
//This file is part of BeagleLib
//BeagleLib is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//BeagleLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along with BeagleLib. If not, see http://www.gnu.org/licenses/.

#include "gpio.h"
#include "register.h"
#include "hw/soc_AM335x.h"
#include "hw/hw_gpio_v2.h"
#include <pthread.h>
#include <poll.h>
#include <string.h>
#include <cassert>
#include <fcntl.h>

#define    CONF_SLEW_SLOW (1<<6)
#define    CONF_RX_ACTIVE (1<<5)
#define    CONF_PULLUP (1<<4)
#define    CONF_PULL_DISABLE (1<<3)
#define    CONF_GPIO_MODE 0x07
#define    CONF_GPIO_OUTPUT CONF_GPIO_MODE
#define    CONF_GPIO_INPUT CONF_GPIO_MODE+CONF_RX_ACTIVE

#define EXPORT_GPIO0 0
#define EXPORT_GPIO1 32
#define EXPORT_GPIO2 64
#define EXPORT_GPIO3 96

#define SYSFS_GPIO_DIR "/sys/class/gpio"

namespace BeagleLib{
  std::map<GPIO_NAME, GPIO_Descriptor> make_gpio_map(){
    std::map<GPIO_NAME, GPIO_Descriptor> res;
    res[USR0] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), -1, 1<<22,		"gpmc_a5"};
    res[USR1] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), -1, 1<<22,           "gpmc_a6"};
    res[USR2] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), -1, 1<<23,           "gpmc_a7"};
    res[USR3] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), -1, 1<<24,           "gpmc_a8"};
    res[GPIO0_7] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_0_REGS), EXPORT_GPIO0+7,  1<<7, "ecap0_in_pwm0_out"};
    res[GPIO0_26] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_0_REGS), EXPORT_GPIO0+26, 1<<26,         "gpmc_ad10"};
    res[GPIO0_27] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_0_REGS), EXPORT_GPIO0+27, 1<<27,         "gpmc_ad11"};
    res[GPIO1_0] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1,     1,          "gpmc_ad0"};
    res[GPIO1_1] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+1,  1<<1,          "gpmc_ad1"};
    res[GPIO1_2] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+2,  1<<2,          "gpmc_ad2"};
    res[GPIO1_3] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+3,  1<<3,          "gpmc_ad3"};
    res[GPIO1_4] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+4, 1<<4,          "gpmc_ad4"};
    res[GPIO1_5] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+5,  1<<5,          "gpmc_ad5"};
    res[GPIO1_6] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+6,  1<<6,          "gpmc_ad6"};
    res[GPIO1_7] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+7,  1<<7,          "gpmc_ad7"};
    res[GPIO1_12] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+12, 1<<12,         "gpmc_ad12"};
    res[GPIO1_13] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+13, 1<<13,         "gpmc_ad13"};
    res[GPIO1_14] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+14, 1<<14,         "gpmc_ad14"};
    res[GPIO1_15] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+15, 1<<15,         "gpmc_ad15"};
    res[GPIO1_16] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+16, 1<<16,           "gpmc_a0"};
    res[GPIO1_17] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+17, 1<<17,           "gpmc_a1"};
    res[GPIO1_28] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+18, 1<<28,         "gpmc_ben1"};
    res[GPIO1_29] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+29, 1<<29,         "gpmc_csn0"};
    res[GPIO1_30] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+30, 1<<30,         "gpmc_csn1"};
    res[GPIO1_31] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_1_REGS), EXPORT_GPIO1+31, 1<<31,         "gpmc_csn2"};
    res[GPIO2_1] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+1,     1,          "gpmc_clk"};
    res[GPIO2_6] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+6,  1<<6,         "lcd_data0"};
    res[GPIO2_7] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+7,  1<<7,         "lcd_data1"};
    res[GPIO2_8] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+8,  1<<8,         "lcd_data2"};
    res[GPIO2_9] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+9,  1<<9,         "lcd_data3"};
    res[GPIO2_10] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+10, 1<<10,         "lcd_data4"};
    res[GPIO2_11] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+11, 1<<11,         "lcd_data5"};
    res[GPIO2_12] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+12, 1<<12,         "lcd_data6"};
    res[GPIO2_13] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+13, 1<<13,         "lcd_data7"};
    res[GPIO2_22] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+22, 1<<22,         "lcd_vsync"};
    res[GPIO2_23] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+23, 1<<23,         "lcd_hsync"};
    res[GPIO2_24] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+24, 1<<24,          "lcd_pclk"};
    res[GPIO2_25] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_2_REGS), EXPORT_GPIO2+25, 1<<25,    "lcd_ac_bias_en"};
    res[GPIO3_19] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_3_REGS), EXPORT_GPIO3+19, 1<<19,        "mcasp0_fsr"};
    res[GPIO3_21] = (struct GPIO_Descriptor){REG_OFFSET(SOC_GPIO_3_REGS), EXPORT_GPIO3+21, 1<<21,     "mcasp0_ahclkx"};
    return res;
  }

  
  //////////////////////////////////////
  // Interrupt Handler
  /////////////////////////////////////

  InterruptHandler::InterruptHandler(void (*callback_interrupt)(void), unsigned int timeout) : timeout_(timeout), callback_interrupt_(callback_interrupt), callback_timeout_(0)
  {
    init();
  }

  InterruptHandler::InterruptHandler(void (*callback_interrupt)(void) , void (*callback_timeout)(void) , unsigned int timeout) : timeout_(timeout), callback_interrupt_(callback_interrupt), callback_timeout_(callback_timeout)
  { 
    init();
  }
  
  InterruptHandler::InterruptHandler(unsigned int timeout) : timeout_(timeout), callback_interrupt_(0), callback_timeout_(0)
  {
    init();
  }
  
  inline void InterruptHandler::init(){
    args.handler = this;
    args.fd = 0;
  }

  void * InterruptHandler::pthread_helper(void* args)
  {
      pthread_arguments* f_args = (pthread_arguments*)args;
      return f_args->handler->interrupt_loop(f_args->fd);
  }

  bool InterruptHandler::is_bound()
  {
    return args.fd>0;
  }

  void InterruptHandler::set_callback_interrupt(void (*f)(void))
  {
    callback_interrupt_ = f;
  }

  void InterruptHandler::set_callback_timeout(void (*f)(void))
  {
    callback_timeout_ = f;
  }

  void InterruptHandler::start_interrupt(const char * gpio_path)
  {
    assert(!is_bound());
    args.fd = open(gpio_path,O_RDONLY | O_NONBLOCK);
    if(args.fd == -1){
      std::cerr <<"Could not open file" << std::endl;
    }
    pthread_create(&thread_,NULL,&InterruptHandler::pthread_helper,&args);
  }

  void InterruptHandler::stop_interrupt()
  {
    assert(is_bound());
    close(args.fd);
    args.fd = 0;
    pthread_cancel(thread_);
  }
  
  void * InterruptHandler::interrupt_loop(int gpio_fd)
  {
    struct pollfd* fdset;
    while(1){
      memset((void*)fdset, 0, sizeof(fdset));
      fdset->fd = gpio_fd;
      fdset->events = POLLPRI;
      int rc = poll(fdset, 1, timeout_);
      if (rc < 0) {
	  std::cerr << "Polling failed on interrupt" << std::endl;
	  return 0;
      }
	    
      if (rc == 0) {
	 callback_timeout_();
      }
      
      if (fdset->revents & POLLIN) {
	  callback_interrupt_();
      }
    }
    return 0;
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
    
    //Export in userspace for consistency/interrupt
    int fd;
    char buf[64];
    int export_no = GPIOs[name_].export_no;
    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
	    std::cerr << "Error exporting pin " << export_no;
    }
    int len = snprintf(buf, sizeof(buf), "%d", export_no);
    write(fd, buf, len);
    close(fd);
  }

  BeagleLib::GPIO< INPUT >::~GPIO()
  {
    //Unexport in userspace
    int fd, len;
    char buf[64];
    int export_no = GPIOs[name_].export_no;
    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
	    std::cerr << "Error unexporting pin " << export_no;
    }
    len = snprintf(buf, sizeof(buf), "%d", export_no);
    write(fd, buf, len);
    close(fd);
  }

  
  DigitalState GPIO< INPUT >::read()
  {
    if(get_reg(GPIOs[name_].mux + GPIO_DATAIN & GPIOs[name_].bit_value)){
      return HIGH;
    }
    return LOW;
  }

  void BeagleLib::GPIO< INPUT >::start_interrupt(InterruptHandler interrupt)
  {
    assert(! interrupt_.is_bound());
    interrupt_ = interrupt;
    
    int export_no = GPIOs[name_].export_no;
    char buf[64];
    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value",export_no);
    interrupt_.start_interrupt((const char *)buf);
  }

  void BeagleLib::GPIO< INPUT >::stop_interrupt()
  {
    assert(interrupt_.is_bound());
    interrupt_.stop_interrupt();
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