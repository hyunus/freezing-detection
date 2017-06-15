#include <Wire.h>
int mpu = 0x68;
int motor = 3;
int buzzer = 9;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int zindex = 1;
int xindex = 1;
int yindex = 1;
int count = 0;
double max1, max2, max3;
int16_t prevz, currentz, postz, prevx, currentx, postx, prevy, currenty, posty;
double maxTime, minTime, stompTime, diff1, diff2;
bool isFreezingOrTurning = false;
bool isFreezing = false;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();
Wire.beginTransmission(mpu);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
pinMode(motor, OUTPUT);
pinMode(buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Wire.beginTransmission(mpu);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(mpu, 14, true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
Tmp=Wire.read()<<8|Wire.read();
GyX=Wire.read()<<8|Wire.read();
GyY=Wire.read()<<8|Wire.read();
GyZ=Wire.read()<<8|Wire.read();

Zscroll();
Xscroll();
Yscroll();
if(isFreezingOrTurning == true && isFreezing == true){
  initializeIntervention();
  resetVariables();
}
  delay(50);
}

void Zscroll(){
 
  if(zindex == 1){
    prevz = GyZ;
  }
  else if(zindex == 2){
    currentz = GyZ;
  }
  else{
    postz = GyZ;
    determineIfMax();
  }
  
  if(max1 != 0 && max2 != 0 && max3 != 0){
    checkPeakDistances();
  }
    prevz = currentz;
    currentz = postz;
    zindex++;
}

void Xscroll(){
  
  if(xindex == 1){
    prevx = AcX;
  }
  else if(xindex == 2){
    currentx = AcX;
  }
  else{
    postx = AcX;
    determineIfMin();
    checkIfOldMin();
    if(isFreezingOrTurning = true && maxTime != 0 && minTime != 0){
      if(abs(maxTime - minTime) < 2.5 && abs((maxTime - stompTime) > 2)){
        isFreezing = true;
      }
      else{
        isFreezingOrTurning = false;
      }
    }
  }
  prevx = currentx;
  currentx = postx;
  xindex++;
}

void Yscroll(){
  if(yindex == 1){
    prevy = GyY;
  }
  else if(yindex == 2){
    currenty = GyY;
  }
  else{
    posty = GyY;
    determineIfStomp();
  }
}

void determineIfMax(){
  if(prevz < currentz && postz < currentz && currentz > 0){
    maxTime = (zindex-1)*0.05;
    if(count == 1){
      max1 = maxTime;
    }
    else if(count == 2){
      max2 = maxTime;
    }
    else if(count == 3){
      max3 = maxTime;
    }
  }
}

void checkPeakDistances(){
  diff1 = abs(max2 - max1);
  diff2 = abs(max3 - max2);
  if(((diff1+diff2)/2) < 2){
    isFreezingOrTurning = true;
  }
    max1 = 0;
    max2 = 0;
    max3 = 0;
    count = 0;
}

void determineIfMin(){
  if(prevx > currentx && postx > currentx && currentx < -5000){
    minTime = (xindex - 1)*0.05;
  }
}

void checkIfOldMin(){
  if(((xindex*0.05) - minTime) > 2){
    minTime = 0;
  }
}

void determineIfStomp(){
  if(prevy > currenty && posty > currenty && currenty < -20000){
    stompTime = (yindex-1)*0.05;
  }
}

void initializeIntervention(){
  analogWrite(motor, 255);
  digitalWrite(buzzer, HIGH);
  delay(250);
  digitalWrite(buzzer,LOW);
  delay(1000);
   digitalWrite(buzzer, HIGH);
  delay(250);
  digitalWrite(buzzer,LOW);
  delay(1000);
   digitalWrite(buzzer, HIGH);
  delay(250);
  digitalWrite(buzzer,LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  delay(250);
  digitalWrite(buzzer,LOW);
  delay(1000);
  analogWrite(motor, 0);
}

void resetVariables(){
  count = 0;
  max1 = 0;
  max2 = 0;
  max3 = 0;
  prevz = 0;
  currentz = 0;
  postz = 0;
  prevx = 0;
  currentx = 0;
  postx = 0;
  maxTime = 0;
  minTime = 0;
  stompTime = 0;
  isFreezingOrTurning = false;
  isFreezing = false;
}

