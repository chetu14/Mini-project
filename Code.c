#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h> 
Servo myservo1; 
SoftwareSerial pc(9, 10); // RX, TX
SoftwareSerial rfidreader(12, 13); // RX, TX
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);//LCD RS-3,En-4,D4-5,D5- 6,D6-7,D7-8
#define oilmotor A2
#define DATAPIN A5    //Data pin
#define CLOCK A4  //ADS 7817 clock
int user=0;
char rfid_data,messgae_letter,pc_data; 
int user1_oil=5,user2_oil=8;
int user1_rice=5,user2_rice=8;
int item_name;
int key;
int bonus;
int f;
char data;
int oil;
long int  adcvalue,c,y,x,z;
int total_amount,user1_amount=25,user2_amount=40;


void setup() 
{ 
pc.begin(9600);
pc.println("SMART RATION SYSTEM");  
rfidreader.begin(9600);
Serial.begin(9600);
pinMode(oilmotor,OUTPUT);
myservo1.attach(A3);  // attaches the servo on pin 9 to the servo 
myservo1.write(40);   
delay(400);
  initspi();
lcdstart();
caliberate();
displaydata();
messagereadinit();
}

void loop() 
{
//displaydata();
 //delay(100); 
if(user==0){rfid(); messageread();}
else if(user==1){user1 ();}        
else if(user==2){user2 ();}    
}

void user1 (void)
{
lcd.clear();lcd.setCursor(0, 0);lcd.print("ENTER "); lcd.print("1-KEROSENE");
lcd.setCursor(0, 1);lcd.print("2-RICE");

pc.println("ENTER "); pc.println("1-KEROSENE 2-RICE");
pc.println("Enter key");
pc_data=0;
do
{
pc_data=softme();//READ DATA FROM KEYBOARD
if(pc_data=='1' || pc_data=='2'){pc.println(" "); pc.println("key is correct");  }
else if(pc_data>10 && pc_data<250) { pc.println(" ");pc.println("Enter correct key");}
}while(!(pc_data=='1' || pc_data=='2')); 
if(pc_data=='1')oil1();
else if(pc_data=='2')rice1();
}
void oil1 (void)
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("KEROSENE BALANCE");lcd.setCursor(0, 1);
lcd.print("IS ");lcd.print(user1_oil); 
pc.print("KEROSENE BALANCE ="); pc.println(user1_oil);

delay(2000);
if(user1_oil>0)//oil section
{
 lcd.clear(); lcd.setCursor(0, 0); lcd.print("ENTER KEROSENE");   
 lcd.setCursor(0, 1); lcd.print("QUANTITY ");  
 pc.println("ENTER KEROSENE QUANTITY ");
pc_data=0;
do
{
do
{
pc_data=softme();
}while(!(pc_data>'0' && pc_data<':'));
key=pc_data-0x30;
if(user1_oil>0)
{
if(user1_oil >=key )
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("processing.....");user=0;
user1_oil=user1_oil-key;digitalWrite(oilmotor, HIGH); delay(key*2000);     digitalWrite(oilmotor, LOW);

total_amount=3*key;
user1_amount=user1_amount-total_amount;
lcd.clear();lcd.setCursor(0, 0);
 lcd.print("KEROSENE BALANCE");lcd.setCursor(0, 1);
lcd.print("IS ");lcd.print(user1_oil); delay(2000);
 message1();total_amount=0;
 lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
 oil=1;

 }
//MORE THAN ASKED
else {
lcd.clear();lcd.setCursor(0, 0); oil=0;
lcd.print("reduce your ");lcd.setCursor(0, 1);lcd.print("quantity & enter");
 pc.println("reduce your quantity & enter");
delay(1000);  
}
}}
while(oil==0);
}
else {user=0;
lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
}  
}

void rice1(void)
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("RICE BALANCE =");lcd.print(user1_rice); delay(2000);
pc.print("RICE BALANCE ="); pc.println(user1_rice);
if(user1_rice>0)//oil section
{
 lcd.clear(); lcd.setCursor(0, 0); lcd.print("ENTER rice");   
 lcd.setCursor(0, 1); lcd.print("QUANTITY ");  
  pc.println("ENTER RICE QUANTITY ");
pc_data=0;
do
{
do
{
pc_data=softme();
}while(!(pc_data>'0' && pc_data<':'));
key=pc_data-0x30;
if(user1_rice>0)
{
if(user1_rice>=key )
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("processing.....");user=0;
user1_rice=user1_rice-key;
myservo1.write(20); 
total_amount=2*key;
user1_amount=user1_amount-total_amount;
key=key*50;caliberate();
do{
 displaydata(); 
 lcd.setCursor(0, 1);lcd.print(y); lcd.print(" grams");  
}while(y<key);
myservo1.write(40); 
 message1(); total_amount=0;
lcd.clear();lcd.setCursor(0, 0);
lcd.print("RICE BALANCE =");lcd.print(user1_rice); delay(2000);
pc.print("RICE BALANCE ="); pc.println(user1_rice);
lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
 oil=1;
 }
//MORE THAN ASKED
else {
lcd.clear();lcd.setCursor(0, 0); oil=0;
lcd.print("reduce your ");lcd.setCursor(0, 1);lcd.print("quantity & enter");
 pc.println("reduce your quantity & enter");
delay(2000);  
}
}}
while(oil==0);
}
else {user=0;
lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
}  
}



void user2 (void)
{
lcd.clear();lcd.setCursor(0, 0);lcd.print("ENTER "); lcd.print("1-KEROSENE");
lcd.setCursor(0, 1);lcd.print("2-RICE");
pc.println("ENTER "); pc.println("1-OIL 2-RICE");
pc.println("Enter key");

pc_data=0;
do
{
pc_data=softme();
if(pc_data=='1' || pc_data=='2'){pc.println(" "); pc.println("key is correct");  }
else if(pc_data>10 && pc_data<250) { pc.println(" ");pc.println("Enter correct key");}
}while(!(pc_data=='1' || pc_data=='2')); 
if(pc_data=='1')oil2();
else if(pc_data=='2')rice2();
}

void oil2 (void)
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("KEROSENE BALANCE ");lcd.setCursor(0, 1);
lcd.print("IS ");lcd.print(user2_oil); delay(2000);
pc.print("KEROSENE BALANCE ="); pc.println(user2_oil);
if(user2_oil>0)//oil section
{
 lcd.clear(); lcd.setCursor(0, 0); lcd.print("ENTER KEROSENE ");   
 lcd.setCursor(0, 1); lcd.print("QUANTITY ");  
pc_data=0;
do
{
do
{
pc_data=softme();

}while(!(pc_data>'0' && pc_data<':'));

key=pc_data-0x30;

if(user2_oil>0)
{

if(user2_oil >=key )
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("processing.....");user=0;
user2_oil=user2_oil-key;digitalWrite(oilmotor, HIGH); delay(key*2000);     digitalWrite(oilmotor, LOW);

 total_amount=3*key;
 user2_amount=user2_amount-total_amount;
lcd.clear();lcd.setCursor(0, 0);
lcd.print("KEROSENE BALANCE ");lcd.setCursor(0, 1);
lcd.print("IS ");lcd.print(user2_oil); 
pc.print("KEROSENE BALANCE ="); pc.println(user2_oil);delay(2000);
 message2(); total_amount=0;
 oil=1;
 lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
 }
//MORE THAN ASKED
else {
lcd.clear();lcd.setCursor(0, 0); oil=0;
lcd.print("reduce your ");lcd.setCursor(0, 1);lcd.print("quantity & enter");
 pc.println("reduce your quantity & enter");
delay(2000);  
}
}}
while(oil==0);
}
else {user=0;
lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
}  
}

void rice2(void)
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("RICE BALANCE =");lcd.print(user2_rice); delay(2000);
pc.print("RICE BALANCE ="); pc.println(user2_rice);
if(user2_rice>0)//oil section
{
 lcd.clear(); lcd.setCursor(0, 0); lcd.print("ENTER rice");   
 lcd.setCursor(0, 1); lcd.print("QUANTITY ");  
   pc.println("ENTER RICE QUANTITY ");
pc_data=0;
do
{
do
{
pc_data=softme();
}while(!(pc_data>'0' && pc_data<':'));
key=pc_data-0x30;
if(user2_rice>0)
{
if(user2_rice>=key )
{
lcd.clear();lcd.setCursor(0, 0);
lcd.print("processing.....");user=0;
user2_rice=user2_rice-key;
myservo1.write(20); 
total_amount=2*key;
 user2_amount=user2_amount-total_amount;

key=key*50;caliberate();
do{
 displaydata(); 
 lcd.setCursor(0, 1);lcd.print(y); lcd.print(" grams");  
}while(y<key);
myservo1.write(40);lcd.clear();lcd.setCursor(0, 0);
lcd.print("RICE BALANCE =");lcd.print(user2_rice); delay(2000);
pc.print("RICE BALANCE ="); pc.println(user2_rice);

lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
 message2(); total_amount=0;
 total_amount=0;
 oil=1;
 }
//MORE THAN ASKED
else {
lcd.clear();lcd.setCursor(0, 0); oil=0;
lcd.print("reduce your ");lcd.setCursor(0, 1);lcd.print("quantity & enter");
 pc.println("reduce your quantity & enter");
delay(2000);  
}
}}
while(oil==0);
}
else {user=0;
lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
 pc.println("swipe your rfid");
}  
}


void rfid(void)
{
  if(rfidreader.available()>0)
{
rfid_data=rfidreader.read();
card1();
card2();
}
}

void card1(void)//card no.100069998C6C (PRINTED NO.10539308) //user 1//card1
{
if(rfid_data=='D')
{    
while(!(rfidreader.available()>0));
rfid_data=rfidreader.read();
if(rfid_data=='1')//IF THIS CARD
{
lcd.clear(); lcd.setCursor(0, 0); lcd.print("Enter password?");
pc.println("Enter password?");
pc_data=0;
do
{
pc_data=softme();//READ DATA FROM KEYBOARD
if(pc_data=='7'){pc.println(" "); pc.println("password is correct");  }
else if(pc_data>10 && pc_data<250) { pc.println(" ");pc.println("Enter correct password");}
}while(!(pc_data=='7')); 
  
lcd.clear(); lcd.setCursor(0, 0); lcd.print("NAME: ARUN");
lcd.setCursor(0, 1);lcd.print("TYPE : APL");
pc.println("NAME: ARUN");
pc.println("TYPE : APL");
delay(3000);
user=1;
}}} 

//card2
void card2(void)//card no.1000697BDDDF (PRINTED NO.10531709)
{
if(rfid_data=='E'){    
while(!(rfidreader.available()>0));
rfid_data=rfidreader.read();
if(rfid_data=='5') //IF THIS CARD
{
pc.println("Enter password?");
lcd.clear(); lcd.setCursor(0, 0); lcd.print("Enter password?");
pc_data=0;
do
{
pc_data=softme();//READ DATA FROM KEYBOARD
if(pc_data=='8'){pc.println(" "); pc.println("password is correct");  }
else if(pc_data>10 && pc_data<250) { pc.println(" ");pc.println("Enter correct password");}
}while(!(pc_data=='8'));   

 lcd.clear(); lcd.setCursor(0, 0); lcd.print("NAME : ABI");
lcd.setCursor(0, 1);lcd.print("TYPE : BPL");
pc.println("NAME : ABI");
pc.println("TYPE : BPL");
user=2; delay(3000);

}}}

void lcdstart(void)
{
lcd.begin(16, 2);// set up the LCD's number of columns and rows:
lcd.setCursor(0, 0);lcd.print("SMART RATION");// Print a message to the LCD.
lcd.setCursor(0, 1);lcd.print("SYSTEM");// Print a message to the LCD.
delay(1000);
lcd.clear();lcd.setCursor(0, 0);
lcd.print("swipe your");lcd.setCursor(0, 1);lcd.print("rfid");
}

void message1(void)
{Serial.print("AT+CMGF=1");//TEXT MODE
Serial.write(0xd);//ENTER
delay(1000);
Serial.print("AT+CMGS=");
Serial.write(0X22);//"
Serial.print("8281964863");//NUMBER
Serial.write(0X22);//"
Serial.write(0xd);//ENTER
delay(1000);
Serial.print("KEROSENE BALANCE IS ");Serial.print(user1_oil);
Serial.print(",RICE BALANCE IS ");Serial.print(user1_rice);
Serial.print(" ,TOTAL AMOUNT IS ");Serial.print(total_amount);
Serial.print(" & BALANCE IS ");Serial.print(user1_amount);
Serial.write(0x1a);//CTRL+Z

}
void message2(void)
{
Serial.print("AT+CMGF=1");//TEXT MODE
Serial.write(0xd);//ENTER
delay(1000);
Serial.print("AT+CMGS=");
Serial.write(0X22);//"
Serial.print("9562868357");//NUMBER
Serial.write(0X22);//"
Serial.write(0xd);//ENTER
delay(1000);
Serial.print("KEROSENE  BALANCE IS ");Serial.print(user2_oil);
Serial.print(",RICE BALANCE IS ");Serial.print(user2_rice);
Serial.print(", TOTAL AMOUNT IS ");Serial.print(total_amount);
Serial.print(" & BALANCE IS ");Serial.print(user2_amount);
Serial.write(0x1a);//CTRL+Z
}
void messagereadinit(void)
{
Serial.print("AT+CMGF=1");//TEXT MODE
Serial.write(0xd);//ENTER
delay(1000);
Serial.print("AT+CNMI=1,2,0,0,0");// //VIEW INCOMING THE MESSAGE DIRECTLY  WITHOUT READING
Serial.write(0xd);
}
void messageread(void)
{
 if(Serial.available()>0)
{
messgae_letter=Serial.read();
 if(messgae_letter=='Z'){
   while(!(Serial.available()>0));
   messgae_letter=Serial.read();
   if(messgae_letter=='Z'){ user1_oil=user1_oil+5;user2_oil=user2_oil+8 ; }
  
 }
}
}
void readADS1231(void) 
{
adcvalue=0;  
//digitalWrite(POWERDOWN, HIGH); //POWER ON
while(digitalRead(DATAPIN));//WAIT UNTIL DATA IS READY
//24-->21
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X800000;} //24
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X400000;}//23
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X200000;}//22
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X100000;}//21
//20-->17
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X80000;} //20
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X40000;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X20000;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X10000;}//17
//16-->13
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X8000;} //16
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X4000;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X2000;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X1000;}//13
//12-->9
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X800;} //12
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X400;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X200;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X100;}//9
//8-->5
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X80;} //8
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X40;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X20;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X10;}//5
//4-->1
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X8;} //4
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X4;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X2;}//
clock ();
if(digitalRead(DATAPIN)){adcvalue=adcvalue+0X1;}//1
clock ();//TO PULL DATA READY TO HIGH STATE --PAGE No.14 ON ADS1231 DATASHEET
//
digitalWrite(CLOCK, LOW);   

}
void caliberate(void)
{
  readADS1231();
 c=(adcvalue*115/53305);
 // Serial.print("caliber=");Serial.println(c);    
}

void displaydata(void)
{
      readADS1231();
 y=(adcvalue*115/53305);//100000
 y=y-c;
readADS1231();
 x=(adcvalue*115/53305);//100000
 x=x-c;
readADS1231();
z=(adcvalue*115/53305);//100000
 z=z-c;
 y=(y+x+z)/3;
// if(y>7000){y=y-7000;}
if(y<0 && y>-20)caliberate();
//Serial.print("weight = ");Serial.print(y); Serial.println(" grams");  
  
}
void clock (void)
{
digitalWrite(CLOCK, LOW);   
digitalWrite(CLOCK, HIGH);
delayMicroseconds(10);

}
void initspi(void)
{

pinMode(DATAPIN, INPUT);
pinMode(CLOCK, OUTPUT);
digitalWrite(CLOCK, LOW);
}

char softme(void)
{
  data=0;
if(digitalRead(2)==0) //checks starting of signal
{ 
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 1;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 2;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 4;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 8;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 16;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 32;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 64;}
delayMicroseconds(102);
if(digitalRead(2)==0)data |= 0;
else{data |= 128;}
}
return data;
}
