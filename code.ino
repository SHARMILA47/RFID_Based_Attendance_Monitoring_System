#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "Wire.h"

#define I2C_ADDRESS 0x68

LiquidCrystal lcd(13,12,11,10,9,8);

#define SW1 A0
#define SW2 A1
#define SW3 A2
#define SW4 A3

int buzzer=6,red_led=5,green_led=4;


char* rfid_id[]={"1900E54250EE","1900E561BC21","18003D312034","14000AD871B7","000003D4EE39","02001047EABF","0200108156C5","0200108040D2","18003D858929","020010811182"};
char* names[]={"LOKESH","NANCY","ALEX","KEVIN","JOJO","PRINCE","TRACY","JIMMY","DANIEL","LAURA"};
int presence[10];

long pm=0;
int i=0,j=0, presentNum=0;


int decToBcd(int val){
  return( (val/10*16) + (val%10) );
}

int bcdToDec(int val){
  return( (val/16*10) + (val%16) );
}

int second, minute, hour, dayOfWeek, dayOfMonth, month, year;
int S=0, M=0, H=0,DOW=0, DOM=0, MONTH=0, YEAR=0;
int Min=0, Hour=0, totMin=0,totHour=0; 


void setup() {   

  Wire.begin();
  Serial.begin(9600);
  lcd.begin(20,4);
  lcd.clear();
  setTime(00,27,15,03,22,05,18);

  pinMode(buzzer,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(SW1,INPUT_PULLUP);
  pinMode(SW2,INPUT_PULLUP);
  pinMode(SW3,INPUT_PULLUP);
  pinMode(SW4,INPUT_PULLUP);

  presentNum=EEPROM.read(1000);
  for(i=0;i<10;i++) 
  presence[i]=EEPROM.read(i);

}
void setTime(int second, int minute, int hour, int dayOfWeek, int dayOfMonth, int month, int year){

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second)); 
  Wire.write(decToBcd(minute)); 
  Wire.write(decToBcd(hour)); 
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month)); 
  Wire.write(decToBcd(year)); 
  Wire.endTransmission();
}
void readTime(int *second,int *minute,int *hour,int *dayOfWeek,int *dayOfMonth,int *month,int *year){

  Wire.beginTransmission(I2C_ADDRESS);Wire.write(0); 
  Wire.endTransmission();Wire.requestFrom(I2C_ADDRESS, 7);

  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime(){

  int HOUR;


  readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);
  if(hour>12)
  HOUR=hour-12;
  else
  HOUR=hour;
  if(j<2){lcd.clear();j++;}

  lcd.setCursor(1,0);
  if (HOUR<10)
  lcd.print("0");

  lcd.print(HOUR);lcd.print(":");

  if (minute<10)
  lcd.print("0");
  lcd.print(minute);
  lcd.print(":");

  if (second<10)
  lcd.print("0");
  lcd.print(second);

  lcd.setCursor(4,1);
  if(hour>12)
  lcd.print("PM");
  else
  lcd.print("AM");

  lcd.setCursor(12,0);
  if (dayOfMonth<10)
  lcd.print("0");
  lcd.print(dayOfMonth);
  lcd.print("/");
  if (month<10)
  lcd.print("0");
  lcd.print(month);
  lcd.print("/");
  lcd.print(year);
  lcd.print(" ");
  lcd.setCursor(11,1);

  switch(dayOfWeek){
  case 1: lcd.print(" Sunday   ");break;
  case 2: lcd.print(" Monday   ");break;
  case 3: lcd.print(" Tuesday  ");break;
  case 4: lcd.print("Wednesday ");break;
  case 5: lcd.print(" Thursday ");break;
  case 6: lcd.print(" Friday   ");break;
  case 7: lcd.print(" Saturday ");break;}


}

void conTime(int a, int b, int c )
{

int Hr;  
int h,  m,  s;
h=EEPROM.read(a);
m=EEPROM.read(b);
s=EEPROM.read(c);

  if(h>12)
  Hr=h-12;
  else
  Hr=h;
  lcd.print(" ");
  if (Hr<10)
  lcd.print("0");

  lcd.print(Hr);lcd.print(":");

  if (m<10)
  lcd.print("0");
  lcd.print(m);
  lcd.print(":");

  if (s<10)
  lcd.print("0");
  lcd.print(s);

  if(h>12)
  lcd.print(" PM");
  else
  lcd.print(" AM");

}






void loop() 
{
  int i;
  char response[12];
  int ch;


top: 

 if(hour==8 && minute==30 && second==0){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("    Clearing");
  lcd.setCursor(2,1);
  lcd.print("Yesterday's data");



  for(i=0;i<10;i++){
  lcd.setCursor(4,3);  
  lcd.print("           ");
  delay(1000);
  lcd.setCursor(4,3);
  lcd.print("Please Wait");
  delay(1000);}
  lcd.clear();
  lcd.setCursor(7,1);
  lcd.print("CLEARED");
  delay(1000);
  EEPROM.write(1000,0);
  for(i=0;i<500;i++)
  EEPROM.write(i,0);

  presentNum=EEPROM.read(1000);
  for(i=0;i<10;i++) 
  presence[i]=EEPROM.read(i);
  j=0;
 }






  displayTime();

  lcd.setCursor(0,3);
  lcd.print("  Press 1 for menu  ");






  if(digitalRead(SW1)==0)
  {
     lcd.clear();lcd.setCursor(0,0);lcd.print(" 1. View attendance ");
     lcd.setCursor(0, 1); lcd.print(" 2. View all");
     lcd.setCursor(0, 2); lcd.print(" 3. Clear all");
     lcd.setCursor(0, 3); lcd.print(" 4. Go Back");
     delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW2)==HIGH && digitalRead(SW3)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW1)== LOW)//View attendance
    {
 top1:lcd.clear(); lcd.setCursor(0,0); lcd.print("Total staffs   - "); lcd.print(10);
      presentNum=EEPROM.read(1000);
      lcd.setCursor(0,1); lcd.print("Staffs present - "); lcd.print(presentNum);
      lcd.setCursor(0,3); lcd.print("1.Detials");
      lcd.setCursor(11,3); lcd.print("4.Go Back");
      delay(500);
      while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear(); goto top;}


     if(digitalRead(SW1)==LOW){
      delay(500);
      for(i=0;i<10;i++) 
      presence[i]=EEPROM.read(i);

      if(presentNum==0){
        lcd.clear();
        lcd.setCursor(2,1);
        lcd.print("No One Is Present");
        delay(2000);
      }
      lcd.clear();
      if(presence[0]==1 || presence[0]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[0]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(300,301,302);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[0]==2)
        conTime(303,304,305);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }
     if(presence[1]==1  || presence[1]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[1]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(310,311,312);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[1]==2)
        conTime(313,314,315);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }
      if(presence[2]==1 || presence[2]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[2]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(320,321,322);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
       if(presence[2]==2)
        conTime(323,324,325);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }

      if(presence[3]==1 || presence[3]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[3]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(330,331,332);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[3]==2)
        conTime(333,334,335);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }
      if(presence[4]==1 || presence[4]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[4]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(340,341,342);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[4]==2)
        conTime(343,344,345);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }


     if(presence[5]==1 || presence[5]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[5]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(350,351,352);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[5]==2)
        conTime(353,354,355);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }

      if(presence[6]==1 || presence[6]==2)
      {
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print(names[6]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(360,361,362);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[6]==2)
        conTime(363,364,365);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }


      if(presence[7]==1 || presence[7]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[7]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(370,371,372);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[7]==2)
        conTime(373,374,375);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }


      if(presence[8]==1 || presence[8]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[8]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(380,381,382);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[8]==2)
        conTime(383,384,385);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }


      if(presence[9]==1 || presence[9]==2)
      {
        lcd.clear();
        lcd.setCursor(7,0);
        lcd.print(names[9]);

        lcd.setCursor(0,1);
        lcd.print("Arrive -");
        conTime(390,391,392);
        lcd.setCursor(0,2);
        lcd.print("Depart -");
        if(presence[9]==2)
        conTime(393,394,395);
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}    
      }
      delay(500); 
      lcd.clear();
      goto top1;
    }

    }



      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;} 

     if(digitalRead(SW2)==LOW){


       lcd.clear();
       lcd.setCursor(3,1);
       lcd.print(" Details From");
       lcd.setCursor(5,2);
       DOM=EEPROM.read(1010);
       if (DOM<10)
       lcd.print("0");
       lcd.print(DOM);
       lcd.print("/");
       MONTH=EEPROM.read(1011);
       if (MONTH<10)
       lcd.print("0");
       lcd.print(MONTH);
       lcd.print("/");
       YEAR=EEPROM.read(1012);
       lcd.print(YEAR);
       delay(3000);




      lcd.clear();lcd.setCursor(7,0); lcd.print(names[0]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[0]==0) lcd.print("Absent");
      if(presence[0]==1) lcd.print("Present");
      if(presence[0]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(900));lcd.print(":");lcd.print(EEPROM.read(901));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  


      lcd.clear();lcd.setCursor(7,0); lcd.print(names[1]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[1]==0) lcd.print("Absent");
      if(presence[1]==1) lcd.print("Present");
      if(presence[1]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(910));lcd.print(":");lcd.print(EEPROM.read(911));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  






      lcd.clear();lcd.setCursor(7,0); lcd.print(names[2]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[2]==0) lcd.print("Absent");
      if(presence[2]==1) lcd.print("Present");
      if(presence[2]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(920));lcd.print(":");lcd.print(EEPROM.read(921));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(7,0); lcd.print(names[3]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[3]==0) lcd.print("Absent");
      if(presence[3]==1) lcd.print("Present");
      if(presence[3]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(930));lcd.print(":");lcd.print(EEPROM.read(931));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(7,0); lcd.print(names[4]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[4]==0) lcd.print("Absent");
      if(presence[4]==1) lcd.print("Present");
      if(presence[4]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(940));lcd.print(":");lcd.print(EEPROM.read(941));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(7,0); lcd.print(names[5]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[5]==0) lcd.print("Absent");
      if(presence[5]==1) lcd.print("Present");
      if(presence[5]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(950));lcd.print(":");lcd.print(EEPROM.read(951));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(5,0); lcd.print(names[6]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[6]==0) lcd.print("Absent");
      if(presence[6]==1) lcd.print("Present");
      if(presence[6]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(960));lcd.print(":");lcd.print(EEPROM.read(961));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(7,0); lcd.print(names[7]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[7]==0) lcd.print("Absent");
      if(presence[7]==1) lcd.print("Present");
      if(presence[7]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(970));lcd.print(":");lcd.print(EEPROM.read(971));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(7,0); lcd.print(names[8]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[8]==0) lcd.print("Absent");
      if(presence[8]==1) lcd.print("Present");
      if(presence[8]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(980));lcd.print(":");lcd.print(EEPROM.read(981));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

      if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}  

      lcd.clear();lcd.setCursor(7,0); lcd.print(names[9]);
      lcd.setCursor(0,1); lcd.print("Status- ");
      if(presence[9]==0) lcd.print("Absent");
      if(presence[9]==1) lcd.print("Present");
      if(presence[9]==2) lcd.print("Left");
      lcd.setCursor(0,2);lcd.print("Total Hours- ");lcd.print(EEPROM.read(990));lcd.print(":");lcd.print(EEPROM.read(991));
        lcd.setCursor(0,3);
        lcd.print("1.Next");
        lcd.setCursor(11,3);
        lcd.print("4.Go Back");
        delay(500);
     while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);
     delay(500);
      lcd.clear();
      goto top;







      } 





     if(digitalRead(SW3)== LOW)
     {
       lcd.clear();
       lcd.setCursor(5,1);
       lcd.print("SHOW ADMIN ID");
       lcd.setCursor(11,3);
       lcd.print("4.Go Back");


       while(!Serial.available()){
        if(digitalRead(SW4)==LOW){
      lcd.clear();
      goto top;}
       }



       if(Serial.available()>0)
      {
        i=0;
        do
        {
            while(Serial.available()<=0);
            response[i]=Serial.read();
            Serial.print(response[i]);
            i++;
        }while(i<12);
        response[i]='\0';



        if(strcmp(response,rfid_id[4])==0)//User1 rfid received
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   ACCESS GRANTED   ");
          lcd.setCursor(0,1);
          lcd.print(" ADMIN - JOJO JOSON");
          analogWrite(buzzer,50); digitalWrite(green_led,HIGH); delay(1000);
          digitalWrite(buzzer,LOW); digitalWrite(green_led,LOW);delay(3000);
          lcd.setCursor(0,3);
          lcd.print("1.Clear");
          lcd.setCursor(11,3);
          lcd.print("4.Go Back");
          delay(500);
          while(digitalRead(SW1)==HIGH && digitalRead(SW4)==HIGH);

          if(digitalRead(SW4)==LOW){
          delay(500);
          lcd.clear();
          goto top;} 



       if(digitalRead(S
