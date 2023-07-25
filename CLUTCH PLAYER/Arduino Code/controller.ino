#include <SoftwareSerial.h>
#include "Ardunity.h"
#include "MPUSeries.h"
#include "DigitalInput.h"
#include "AnalogInput.h"

SoftwareSerial swSerial(5, 3);
signed char orient_mpu6500_7[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
MPUSeries mpu6500_7(7, MPU6500, false, orient_mpu6500_7);
DigitalInput dInput2(2, 2, true);
AnalogInput aInput0(0, A1);
DigitalInput dInput3(3, 7, true);
AnalogInput aInput1(1, A0);
DigitalInput dInput4(4, 6, true);

void setup()
{
  ArdunityApp.attachController((ArdunityController*)&mpu6500_7);
  ArdunityApp.attachController((ArdunityController*)&dInput2);
  ArdunityApp.attachController((ArdunityController*)&aInput0);
  ArdunityApp.attachController((ArdunityController*)&dInput3);
  ArdunityApp.attachController((ArdunityController*)&aInput1);
  ArdunityApp.attachController((ArdunityController*)&dInput4);
  ArdunityApp.resolution(256, 1024);
  ArdunityApp.timeout(5000);
  swSerial.begin(9600);
  ArdunityApp.begin((Stream*)&swSerial);
}

void loop()
{
  ArdunityApp.process();
}
