//Copyright 2012 Philippe Tillet
//This file is part of BeagleLib
//BeagleLib is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//BeagleLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along with BeagleLib. If not, see http://www.gnu.org/licenses/.

#ifndef EQEP_HPP
#define EQEP_HPP

#include "register.h"

//EQEP 1:
//	A : GPIO1_20
//	B : GPIO1_21

//EQEP 2:
//	A : GPIO1_12
//	B : GPIO1_13


namespace BeagleLib{


  
  enum EQEP_ID{
    EQEP0 = 0,
    EQEP1 = 1,
    EQEP2 = 2
  };
  
  class EQEP{
  private:
    EQEP_ID id_;
  public:
    EQEP(EQEP_ID id);
    int32_t value();
  };
  
}

#endif