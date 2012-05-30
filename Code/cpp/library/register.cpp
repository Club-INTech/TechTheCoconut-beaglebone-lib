//Copyright 2012 Philippe Tillet
//This file is part of BeagleLib
//BeagleLib is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//BeagleLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along with BeagleLib. If not, see http://www.gnu.org/licenses/.

#include <stdint.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include "register.h"

namespace BeagleLib{

    int get_fd_mem(){
      static int fd = open("/dev/mem",O_RDWR);
    }

    int is_big_endian(void){
	union {
	    uint32_t i;
	    char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1; 
    }

    inline void endian_swap(uint32_t& x)
    {
	x = (x>>24) | 
	    ((x<<8) & 0x00FF0000) |
	    ((x>>8) & 0x0000FF00) |
	    (x<<24);
    }

    inline void to_little_endian(uint32_t& x){
      if(is_big_endian()){
	endian_swap(x);
      }
    }

    inline void to_big_endian(uint32_t& x){
      if(!is_big_endian()){
	endian_swap(x);
      }
    }

    void set_reg(uint32_t address, uint32_t new_value){
      uint8_t* map;
      if (get_fd_mem() == -1) {
	    perror("Error opening file for writing");
	    exit(EXIT_FAILURE);
      }
      map = (uint8_t *)mmap(0, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, get_fd_mem(), MMAP_OFFSET);
      uint32_t* reg = (uint32_t*)(map+address);
      to_little_endian(new_value);
      *reg = new_value;
    }


    uint32_t get_reg(uint32_t address){
      uint8_t* map;
      if (get_fd_mem() == -1) {
	    perror("Error opening file for writing");
	    exit(EXIT_FAILURE);
      }
      map = (uint8_t*) mmap(0, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, get_fd_mem(), MMAP_OFFSET);
      uint32_t* reg = (uint32_t*)(map+address);
      to_little_endian(*reg);
      return *reg;
    }
    
    void or_reg(uint32_t address, uint32_t mask){
      set_reg(address,get_reg(address) | mask);
    }
    
    void and_reg(uint32_t address, uint32_t mask){
      set_reg(address,get_reg(address) & mask);
    }
    
    void xor_reg(uint32_t address, uint32_t mask){
      set_reg(address,get_reg(address) ^ mask);
    }
    
    void clear_reg(uint32_t address, uint32_t mask){
      and_reg(address,~mask);
    }
    
    void pin_mux(std::string const & fn, unsigned int mode){
      std::string path = "/sys/kernel/debug/omap_mux/";
      path+=fn;
      std::ofstream f(path.c_str());
      if(!f){
	std::cerr << "Error Opening " << path << std::endl;
	return;
      }
      f << std::hex << mode;
      f.close();
    }
}