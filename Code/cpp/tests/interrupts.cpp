#include <gpio.h>
#include <iostream>

using namespace BeagleLib;

void callback_interrupt(){
  std::cout << "Interrupt !" << std::endl;
}

void callback_timeout(){
  std::cout << "Timeout!" << std::endl;
}
int main(){
  GPIO< INPUT > gpio(GPIO1_1);
  gpio.start_interrupt(InterruptHandler(callback_interrupt,callback_timeout));
}