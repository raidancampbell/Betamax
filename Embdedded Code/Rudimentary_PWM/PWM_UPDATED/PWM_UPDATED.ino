//PWM queries at 1KHz
int cyclelength = 40;
volatile int doody = 1;
int current = 0;

void setup(){
  //set pins as outputs
  pinMode(11, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
cli();//stop interrupts
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  OCR0A = 249;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01);// | (1 << CS00);   
  //TCCR0B |= (1 << CS02) | (1 << CS00); //256 prescaler!
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
sei();//allow interrupts

}//end setup

ISR(TIMER0_COMPA_vect){
  current++;
  if(current <= doody) digitalWrite(11,HIGH);
  if(current > doody) digitalWrite(11,  LOW);
  if(current > cyclelength) current = 0;
}

boolean toggle=LOW;
int q, z, y;

void loop(){

  for(doody = 0; doody < 40; doody++){
    for (q = 0; q < 30000; q++) {
      for (z=0 ; z<10 ; z++) {
        y = z*q;
      }
    }
    digitalWrite(9,toggle);
    toggle = !toggle;
  }
   for(doody = 40; doody > 0; doody--){
    for (q = 0; q < 30000; q++) {
      for (z=0 ; z<10 ; z++) {
        y = z*q;
      }
    }
    digitalWrite(9,toggle);
    toggle = !toggle;
  }
}
