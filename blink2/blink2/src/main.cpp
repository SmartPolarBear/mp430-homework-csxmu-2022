#include <msp430g2553.h>

void set_1mhz()
{
  BCSCTL1 = CALBC1_1MHZ; // Set range

  DCOCTL = CALDCO_1MHZ;

  BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz
}

void set_8mhz()
{
  BCSCTL1 = CALBC1_8MHZ; // Set range

  DCOCTL = CALBC1_8MHZ;

  BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz
}

int main()
{
  WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
  P1DIR |= 0x01;            // configure P1.0 as output

  set_1mhz();

  volatile unsigned int i; // volatile to prevent optimization

  while (1)
  {
    P1OUT ^= 0x01; // toggle P1.0
    for (i = 10000; i > 0; i--)
      __delay_cycles(10); // delay
  }
  return 0;
}
