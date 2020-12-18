#include <AccelStepper.h>

AccelStepper Stepper1(1,11,10); //использует пин 10 и 11 для dir и step, 1 - режим "external driver" (A4988)
AccelStepper Stepper2(1,9,8);

#define SPEED   7
#define DIR     6

int xs;
int ys;
int xf;
int yf;
int xc = 48;
int yc = 48;
int step_ = 450;
String axis = "none";
String xyStr = "";

void setup() {
  Serial.begin(9600);
  
  Stepper1.setMaxSpeed(3000); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  Stepper1.setAcceleration(13000); //устанавливаем ускорение (шагов/секунду^2)
  Stepper2.setMaxSpeed(3000);
  Stepper2.setAcceleration(13000);

  // take off the platform
  pinMode(DIR, OUTPUT); 
  pinMode(SPEED, OUTPUT); 
  digitalWrite(DIR, HIGH);
  digitalWrite(SPEED, HIGH);
  delay(3000);
  // put on the platform while waiting
  digitalWrite(SPEED, LOW);
  delay(5000);
  // lower a magnet
  digitalWrite(DIR, LOW);
  digitalWrite(SPEED, HIGH);
  delay(1700);
  digitalWrite(SPEED, LOW);
  delay(1000);
}

void loop() {
      // + направление - против часовой стрелки || ++: x || --: -x || +-: y || -+: -y
if(Serial.available() > 0){
  xyStr = Serial.readString();
  xs = xyStr[0];
  ys = xyStr[2];
  xf = xyStr[4];
  yf = xyStr[6];

  axis = "xs";
  }
  
  // go to the player coordinates
if(axis == "xs" and Stepper1.distanceToGo() == 0){
    axis = "ys";
    Stepper1.move(step_ * (xs-xc));
    Stepper2.move(step_ * (xs-xc));
    }
if(axis == "ys" and Stepper1.distanceToGo()== 0){
    Stepper1.move(step_ * (ys-yc));
    Stepper2.move(step_ * (ys-yc)* (-1));
    axis = "xf";
    }
    
  // move player to specified coordinates
if(axis == "xf" and Stepper1.distanceToGo() == 0){
    axis = "yf";
    Stepper1.move(step_ * (xf-xs));
    Stepper2.move(step_ * (xf-xs));

    digitalWrite(DIR, HIGH);
    digitalWrite(SPEED, HIGH);
    delay(1500);
    digitalWrite(SPEED, LOW);
    delay(1000);
    }
if(axis == "yf" and Stepper1.distanceToGo()== 0){
    Stepper1.move(step_ * (yf-ys));
    Stepper2.move(step_ * (yf-ys)* (-1));
    axis = "lowerMag";

    xc = xf;
    yc = yf;
    }
if(axis == "lowerMag" and Stepper1.distanceToGo()== 0){
    digitalWrite(DIR, LOW);
    digitalWrite(SPEED, HIGH);
    delay(1500);
    digitalWrite(SPEED, LOW);
    delay(1000);
    axis = "none";
}

  Stepper1.run();
  Stepper2.run();

}
