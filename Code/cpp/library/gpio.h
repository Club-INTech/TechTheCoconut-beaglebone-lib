/** @file gpio.h
 *  @brief File for handling GPIOs of the Beaglebone
 */

//Copyright 2012 Philippe Tillet
//This file is part of BeagleLib
//BeagleLib is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//BeagleLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along with BeagleLib. If not, see http://www.gnu.org/licenses/.


#ifndef BEAGLELIB_GPIO_H
#define BEAGLELIB_GPIO_H

#include <map>
#include <stdint.h>
#include <string>
#include "register.h"

namespace BeagleLib{
  
  /**
   * @enum GPIO_NAME
   * @brief Names of the GPIO pins
   */
  enum GPIO_NAME{
    USR0,
    USR1,
    USR2,
    USR3,
    GPIO0_7,
    GPIO0_26,
    GPIO0_27,
    GPIO1_0,
    GPIO1_1,
    GPIO1_2,
    GPIO1_3,
    GPIO1_4,
    GPIO1_5,
    GPIO1_6,
    GPIO1_7,
    GPIO1_12,
    GPIO1_13,
    GPIO1_14,
    GPIO1_15,
    GPIO1_16,
    GPIO1_17,
    GPIO1_28,
    GPIO1_29,
    GPIO1_30,
    GPIO1_31,
    GPIO2_1,
    GPIO2_6,
    GPIO2_7,
    GPIO2_8,
    GPIO2_9,
    GPIO2_10,
    GPIO2_11,
    GPIO2_12,
    GPIO2_13,
    GPIO2_22,
    GPIO2_23,
    GPIO2_24,
    GPIO2_25,
    GPIO3_19,
    GPIO3_21,
  };
  
  /**
   * @enum GPIO_MODE
   * @brief Modes for the GPIO Pins
   */
  enum GPIO_MODE{
    INPUT,
    OUTPUT,
    PWM
  };
  
  /**
  * @enum PULL
  * @brief PULL Modes for the GPIOs
  */
  enum PULL{
     PULLUP,
     PULLDOWN,
     NOPULL
   };
   
  /**
  * @enum DigitalState
  * @brief States of a Digital Pin.
  */
  enum DigitalState{
    LOW = 0,
    HIGH = 1
  };
  
   struct GPIO_Descriptor{
    int mux;
    uint32_t bit_value;
    std::string pinmux_filename;
   };
   
  std::map<GPIO_NAME, GPIO_Descriptor> make_gpio_map();
  static std::map<GPIO_NAME,GPIO_Descriptor> GPIOs = make_gpio_map();
  
  /** @class GPIO
   *  @brief Class for the GPIO Pins
   *  @tparam mode Mode from the enum GPIO_MODE
   */
  template<GPIO_MODE mode>
  class GPIO;
  
  template<>
  class GPIO<OUTPUT>{
  private:
      GPIO_NAME name_;  
  public:
      GPIO(GPIO_NAME name);
      void write(DigitalState val);
      void toggle();
  };
  
  template<>
  class GPIO<INPUT>{
  private:
      GPIO_NAME name_;  
  public:
      GPIO(GPIO_NAME name, PULL PullMode = PULLDOWN);
      DigitalState read();
  };


}

#endif