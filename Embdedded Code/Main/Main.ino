#include <virtuabotixRTC.h>
#define PWRSW 9
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 11
#define LED7 12
#define LED8 13
#define LED9 14
#define LED10 15
#define LED11 16
#define LED12 17
// Creation of the Real Time Clock Object
//SCLK -> 6, I/O -> 7, CE -> 8
virtuabotixRTC myRTC(26, 27, 28);
boolean l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12;

void setup() { 
  pinMode(LED1, OUTPUT);
  digitalWrite(LED2, LOW);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED3, LOW);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED4, LOW);
  pinMode(LED4, OUTPUT);
  digitalWrite(LED5, LOW);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED6, LOW);
  pinMode(LED6, OUTPUT);
  digitalWrite(LED7, LOW);
  pinMode(LED7, OUTPUT);
  digitalWrite(LED8, LOW);
  pinMode(LED8, OUTPUT);
  digitalWrite(LED9, LOW);
  pinMode(LED9, OUTPUT);
  digitalWrite(LED10, LOW);
  pinMode(LED10, OUTPUT);
  digitalWrite(LED11, LOW);
  pinMode(LED11, OUTPUT);
  digitalWrite(LED12, LOW);
  pinMode(LED12, OUTPUT);
  digitalWrite(LED1, LOW);
}

void loop() { 
// Grab the current time
myRTC.updateTime(); 
if(myRTC.minutes == 0) shift();
if(digitalRead(PWRSW) == HIGH) off();
else on();
delay(1000); 
}

void shift() {//cleverish, but should be optimized to eliminate being on for 12 cycles
  l1 = l12;
  l2 = l1;
  l3 = l2;
  l4 = l3;
  l5 = l4;
  l6 = l5;
  l7 = l6;
  l8 = l7;
  l9 = l8;
  l10 = l9;
  l11 = l10;
  l12 = l11;
}
void off() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
  digitalWrite(LED9, LOW);
  digitalWrite(LED10, LOW);
  digitalWrite(LED11, LOW);
  digitalWrite(LED12, LOW);
}
void on() {
  digitalWrite(LED1, l1);
  digitalWrite(LED2, l2);
  digitalWrite(LED3, l3);
  digitalWrite(LED4, l4);
  digitalWrite(LED5, l5);
  digitalWrite(LED6, l6);
  digitalWrite(LED7, l7);
  digitalWrite(LED8, l8);
  digitalWrite(LED9, l9);
  digitalWrite(LED10, l10);
  digitalWrite(LED11, l11);
  digitalWrite(LED12, l12);
}
