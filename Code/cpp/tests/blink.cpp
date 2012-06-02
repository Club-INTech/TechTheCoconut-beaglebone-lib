#include <gpio.h>
#include <time.h>

using namespace BeagleLib;

int main(){
  GPIO<OUTPUT> led1(USR2);
  GPIO<OUTPUT> led2(USR3);

  led1.write(HIGH);
  led2.write(LOW);
  
  while(1){
    led1.toggle();
    led2.toggle();
    sleep(1);
  }

}