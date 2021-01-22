#include <LiquidCrystal.h>
float voltP=0, maxV=0, maxI=0, currP=0, voltRms=0, currRms=0;
float Rv1=3300, Rv2=1000, shunt=5.7, currerror=0.0008, Gain=6.6;
float ontime, offtime, freq, deltat,anR, anD, pf;
float appPow, actPow, reaPow;

LiquidCrystal LCD(4,6,10,11,12,13);

void setup(){
  Serial.begin(9600);
  LCD.begin(16,2);
  LCD.print("Digital");
  LCD.setCursor(0,1);
  LCD.print("Wattmeter");
  delay(1500);
   pinMode(7,INPUT);
  pinMode (3,INPUT);
}

void loop() {
  for(int i=0; i<1000; i++)
  {
    voltP=analogRead(A0);
    if(voltP>maxV)
    maxV=voltP;
  }
  for(int i=0; i<1000; i++)
  {
    currP=analogRead(A1);
    if(currP>maxI)
    maxI=currP;
  }
  maxV=maxV*5.0/1024;
  voltP=maxV*(Rv1+Rv2)/Rv1;
  voltP=voltP+0.7;
  voltRms=voltP*0.707;
  if(voltRms>1.3)
  voltRms+=2.5;
  else if(voltRms>1)
  voltRms+=1;
  else if(voltRms>0.5)
  voltRms+=0.5;
  LCD.clear();
  LCD.print("Voltage");
  LCD.setCursor(0,1);
  LCD.print(voltRms); LCD.print("V");
  delay(1500);
  Serial.print(voltRms);
  maxI=maxI*5.0/1024;
  currP=maxI/Gain;
  currP=currP/shunt;
  currRms=currP*0.707-currerror;
  LCD.clear();
  LCD.print("Current");
  LCD.setCursor(0,1);
  LCD.print(currRms*1000); LCD.print("mA");
  delay(1500);

  ontime=pulseIn(3,HIGH);
  offtime=pulseIn(3,LOW);
  freq=ontime+offtime;
  freq=1000000/freq;
  LCD.clear();
  LCD.print("freq");
  LCD.setCursor(0,1);
  if(freq<1000)
  {
  LCD.print(freq); LCD.print("Hz");
  }
  else
  {
  LCD.print(freq/1000); LCD.print("KHz");
  }
  delay(1500);
  LCD.clear();
  ontime=pulseIn(7,HIGH);
  deltat=ontime;
  deltat=deltat/1000000;
  LCD.print("time del"); 
  LCD.setCursor(0,1);
  LCD.print(deltat*1000);
  LCD.print("ms");
  delay(1500);
  anD=deltat*freq*360;
  anR=anD/57.3;
  pf=cos(anR);
  if(pf<0)
  pf=-pf;
  LCD.clear();
  LCD.print("pf");
  LCD.setCursor(0,1);
  LCD.print(pf);
  delay(1500);

  actPow=voltRms*currRms*pf;
  appPow=voltRms*currRms;
  reaPow=voltRms*currRms*sin(anR);
  if(actPow>0.01)
  {
  LCD.clear();
  LCD.print("Active Power");
  LCD.setCursor(0,1);
  LCD.print(actPow); LCD.print("W");
  delay(1500);
  }
  else
  {
    LCD.clear();
    LCD.print("Active Power");
  LCD.setCursor(0,1);
  LCD.print(actPow*1000); LCD.print("mW");
  delay(1500);
  
  }
  if(reaPow>0.01)
  {
  LCD.clear();
  LCD.print("Reactive Power");
  LCD.setCursor(0,1);
  LCD.print(reaPow); LCD.print("VAR");
  delay(1500);
  }
  else
  {  LCD.clear();
  LCD.print("Reactive Power");
  LCD.setCursor(0,1);
  LCD.print(reaPow*1000); LCD.print("mVAR");
  delay(1500);
  }
  if(appPow>0.01)
  {
  LCD.clear();
  LCD.print("Apparent Power");
  LCD.setCursor(0,1);
  LCD.print(appPow); LCD.print("VA");
  delay(1500);
  }
  else
  { 
  LCD.clear();
  LCD.print("Apparent Power");
  LCD.setCursor(0,1);
  LCD.print(appPow*1000); LCD.print("mVA");
  delay(1500);
  }

}
