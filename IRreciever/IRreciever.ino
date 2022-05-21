//IR // On Atmega328P!!! 16Mhz
///////////////////////////////////////////////////////////////
#define OCR_VAL 159
#define highSignal 0
#define lowSignal 1
#include <avr/io.h>
#include <avr/interrupt.h>
///////////////////////////////////////////////////////////////
volatile bool started = false;
///////////////////////////////////////////////////////////////
void setupTimer();
void setupExternalInterrupt();
void convert();
//////////////////////////////////////////////////////////////
volatile unsigned int timeKeep;
volatile unsigned int data[20];
volatile unsigned int nextSignalTime;
volatile bool nextSignal;
volatile bool state = false;
volatile unsigned int num = 0;
volatile unsigned int inc = 0;
volatile unsigned int botID = 0;
///////////////////////////////////////////////////////////////
ISR(TIMER0_COMPA_vect)
{
  timeKeep++;
}
///////////////////////////////////////////////////////////////
ISR(INT0_vect)
{
  nextSignalTime = timeKeep;
  EIMSK &= ~_BV(INT0);

  sei();

  timeKeep = 0;
  TCNT0 = 0;

  if (inc == 16)
    state = true;

  if ((nextSignalTime > 360) && (nextSignalTime < 440) && started)
  {
    Serial.println(String(num)+","+String(botID));
    reset();
  }

  else if (nextSignal == highSignal)
  {
    nextSignal = lowSignal;
    EICRA &= (~((1 << ISC01) | (1 << ISC00)));
    EICRA = _BV(ISC01);
  }

  else if (nextSignal == lowSignal)
  {
    nextSignal = highSignal;
    EICRA |= _BV(ISC00);
  }

  if ((nextSignalTime > 260) && (nextSignalTime < 340))
    started = true;

  else if ((nextSignalTime > 60) && (nextSignalTime < 140))
  {
    if (started)
    {
      if (state)
      {
        botID <<= 1;
        botID += 0;
      }
      else
      {
        num <<= 1;
        num += 0;
      }
      inc++;
    }

  }

  else if ((nextSignalTime > 160) && (nextSignalTime < 240))
  {
    if (started)
    {
      if (state)
      {
        botID <<= 1;
        botID += 1;
      }
      else
      {
        num <<= 1;
        num += 1;
      }
      inc++;
    }

  }
  /*else
    reset();*/




  EIMSK |= _BV(INT0);

}
///////////////////////////////////////////////////////////////
int main()
{
  init();
  Serial.begin(9600);

  PORTD = 0x0;

  setupTimer();
  setupExternalInterrupt();

  nextSignal = highSignal;

  sei();


  while (1)
  {

  }
  return 0;
}
///////////////////////////////////////////////////////////////
void setupTimer()
{
  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS00);
  TIMSK0 = _BV(OCIE0A);
  OCR0A = OCR_VAL;
}
///////////////////////////////////////////////////////////////
void setupExternalInterrupt()
{
  EIMSK = _BV(INT0); //External interrupt on PD2
  EICRA = _BV(ISC00) | _BV(ISC01);
}
//////////////////////////////////////////////////////////////
void reset()
{
  nextSignal = highSignal;   //We expect high signal for the data to be received
  started = false;

  for (num = 0; num < 20; num++)
    data[num] = 0;
  num = 0;
  inc = 0;
  state = false;
  botID = 0;
  EICRA = (_BV(ISC00) | _BV(ISC01));
}
///////////////////////////////////////////////////////////////
