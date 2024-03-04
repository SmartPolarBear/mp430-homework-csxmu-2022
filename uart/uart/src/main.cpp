#include <msp430g2553.h>
#include <stdint.h>
#include <stdbool.h>

void set_1mhz()
{
  BCSCTL1 = CALBC1_1MHZ; // Set range

  DCOCTL = CALDCO_1MHZ;
}

void uart_init()
{
  UCA0CTL1 |= UCSSEL_2;
  UCA0BR0 = 104;
  UCA0BR1 = 0;
  UCA0MCTL = UCBRS0;
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;
}

void uart_puts(char *c)
{
  while (*c)
  {
    while (!(IFG2 & UCA0TXIFG))
    {
      __nop();
    }
    UCA0TXBUF = *c++;
  }
}

void __attribute__((interrupt(USCIAB0RX_VECTOR))) uart_rx_isr()
{
  volatile int led_on = !!UCA0RXBUF;

  if (led_on)
  {
    P1OUT |= BIT0;
    uart_puts("LED ON\n");
  }
  else
  {
    P1OUT &= ~BIT0;
    uart_puts("LED OFF\n");
  }
}

int main()
{
  WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

  P1DIR |= BIT0; // configure P1.0 as output
  P1OUT &= ~BIT0;

  DCOCTL = 0;
  set_1mhz();

  P1SEL = BIT1 | BIT2;
  P1SEL2 = BIT1 | BIT2;

  uart_init();

  __bis_SR_register(LPM0_bits | GIE);
  return 0;
}
