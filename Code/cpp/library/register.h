//Copyright 2012 Philippe Tillet
//This file is part of BeagleLib
//BeagleLib is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//BeagleLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along with BeagleLib. If not, see http://www.gnu.org/licenses/.


#ifndef BEAGLELIB_REGISTER_H
#define BEAGLELIB_REGISTER_H

#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

namespace BeagleLib{


     static const unsigned int MMAP_OFFSET = 0x44c00000 ;
     static const unsigned int MMAP_SIZE   = 0x48ffffff-MMAP_OFFSET;
       
    class MMapHandler{
    private:
       int fd_;
       uint8_t* map_;
    public:
      MMapHandler();
      uint8_t * get(){ return map_; }
      ~MMapHandler();
    };
    
    int is_big_endian(void);
    inline void endian_swap(uint32_t& x);
    inline void to_little_endian(uint32_t& x);
    inline void to_big_endian(uint32_t& x);
    
    void set_reg(uint32_t address, uint32_t new_value);
    uint32_t get_reg(uint32_t address);
    void or_reg(uint32_t address, uint32_t mask);
    void and_reg(uint32_t address, uint32_t mask);
    void xor_reg(uint32_t address, uint32_t mask);
    void clear_reg(uint32_t address, uint32_t mask);
    void pin_mux(std::string const & fn, unsigned int mode);
}
#endif