//On ATMEGA32 !! clk 1Mhz internal
#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#define DATAFRAME 20
volatile int i;
////////////////////////////////////////////////////////////////////////
/*ISR(TIMER1_COMPA_vect)
  {
  TCCR0 ^= _BV(COM00);
  }*/
int data[DATAFRAME];
void convert(int num, int botID)
{
  int index = (DATAFRAME-5);
  while (index >= 0)
  {
    data[index] = num & 1;
    index--;
    num >>= 1;
  }
  index = DATAFRAME-1;
  
  while (index >= DATAFRAME-4)
  {
    data[index] = botID & 1;
    index--;
    botID >>= 1;
  }
  

}
////////////////////////////////////////////////////////////////////////
void send1()
{
  //To Signify Starting Bit
  TCCR0 ^= _BV(COM00);
  _delay_ms(3);
  
  for (i = 0; i < DATAFRAME; i++)
  {
    TCCR0 ^= _BV(COM00);
    _delay_ms(1);
    if (data[i])
      _delay_ms(1);   
  }

  TCCR0 ^= _BV(COM00);
  _delay_ms(4);
  TCCR0 ^= _BV(COM00);
}

////////////////////////////////////////////////////////////////////////
int main()
{

  DDRB = _BV(DDB3);
  OCR0 = 103; //38.5KHz
  //TCCR0 |= _BV(COM00);
  TCCR0 |= _BV(WGM01);
  TCCR0 |= _BV(CS00);

  /*OCR1A = 8999; // clk = 1Mhz
    TCCR1B = _BV(WGM12) | _BV(CS10);
    TIMSK = _BV(OCIE1A);*/

  sei();


  while (1)
  {
    convert(67,4);//Put here the data to send
    send1();
    _delay_ms(1500);
  }

  return 0;
}
////////////////////////////////////////////////////////////////////////////////
