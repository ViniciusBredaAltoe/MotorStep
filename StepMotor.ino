#include <Stepper.h>

#define STEPS 100
#define resolution 2 //Averiguar!!

 

Stepper stepper(STEPS, 8, 9, 10, 11);


int fcr = 22;
int fcl = 24;
int botton_right = 26;
int botton_go = 28;
int bottom_left = 30;
int bottom_mode = 32;

void setup() {
  stepper.setSpeed(30); // set the speed of the motor to 30 RPMs
  int cont = 0;
  int Dmax;
  while(digitalRead(fcr) == LOW){
    stepper.step(1);
  }
  while(digitalRead(fcl) == LOW){
    stepper.step(-1);
    cont++;
  }
  //Printar Cont
  Dmax = cont*resolution;
  cont = 0;
  
}

void loop() {
}
