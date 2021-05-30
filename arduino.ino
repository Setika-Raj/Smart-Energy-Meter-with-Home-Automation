#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


SoftwareSerial transmit(9,10); //rx,tx
LiquidCrystal lcd(12, 13, 7,6,5,4);//rs, en, d4, d5, d6, d7

float sensitivity = 0.1; //100mv for 20ampere acs712
float vpp = 0.0048828125; //voltage per unit=5/1024
float costPersec = 0.11; //rupees
float costPerinterval=0.3l;
int small = 1;
int big=1;
float V = 220;

float Power = 0;
float current_after_interval;
float currentSum = 0; //collecting current samples over an interval
float avg_current_during_interval = 0; //calculating avg current of current samples during an interval to obtain a constant value

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  transmit.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);
}
void loop() {
 //for big interval 
        for(big;big<=3;big++){
  //for small interval
for(small;small<=10*60;small++){
//for 1 sensor
      float sensorVa = 2.5 - (analogRead(A0) + 5) * vpp;//calculating acs712 hall voltage and subtracting offset V=2.5 to remove polarity of voltage
      float abs_sensorVa = abs(sensorVa);
      float Ia = abs_sensorVa / sensitivity;
//for 2 sensor
      float sensorVb = 2.5 - (analogRead(A1) + 5) * vpp;//calculating acs712 hall voltage and subtracting offset V=2.5 to remove polarity of voltage
      float abs_sensorVb = abs(sensorVb);
      float Ib = abs_sensorVb / sensitivity;
float I = Ia + Ib;

currentSum += I;//collecting current samples over an interval
      avg_current_during_interval = currentSum / small; //current averaged over samples within given interval

     float cum_pow=(V * avg_current_during_interval + Power);//cumulative power 
      show(cum_pow);//displaying cumlative power on lcd
transmit.write(int(cum_pow));//transmitting to nodemcu
delay(1000);
}

small=1;
current_after_interval += avg_current_during_interval; //current after a given interval
Power = V * current_after_interval;
float predictedInterval=(Power/big)*3;//estimated power for big interval from small interval
//printing msg to send to python
String msg="power="+String(Power)+"\nCost="+String(Power*costPerinterval)+"\npredicted power="+String(predictedInterval)+"\n\n\n";
Serial.println(msg);
currentSum = 0;
}
//resetting every value for starting the big interval again
Power=0;
big=1;
current_after_interval=0;
currentSum=0;
}
//displaying on lcd
void show(float power) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("POWER:" + String(power) + "W");

  lcd.setCursor(0, 1);
  lcd.print("Cost:Rs." + String(power * costPersec));



}
