#include <msp430g2553.h>

int IncDec_PWM = 1;

int main(void)
{

  /*** Watchdog timer and clock Set-Up ***/
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  DCOCTL = 0;               // Select lowest DCOx and MODx
  BCSCTL1 = CALBC1_1MHZ;    // Set range
  DCOCTL = CALDCO_1MHZ;     // Set DCO step + modulation

  P1DIR |= BIT6;
  P1SEL |= BIT6;

  /*** Timer0_A Set-Up ***/
  TA0CCR0 |= 1000;           // PWM period
  TA0CCR1 |= 1;              // TA0CCR1 PWM duty cycle
  TA0CCTL1 |= OUTMOD_7;      // TA0CCR1 output mode = reset/set
  TA0CTL |= TASSEL_2 + MC_1; // SMCLK, Up Mode (Counts to TA0CCR0)

  /*** Timer1_A Set-Up ***/
  TA1CCR0 |= 2000;           // Counter value
  TA1CCTL0 |= CCIE;          // Enable Timer1_A interrupts
  TA1CTL |= TASSEL_2 + MC_1; // SMCLK, Up Mode (Counts to TA1CCR0)

  _BIS_SR(LPM0_bits + GIE); // Enter Low power mode 0 with interrupts enabled

  return 0;
}

#pragma vector = TIMER1_A0_VECTOR // Timer1 A0 interrupt service routine
__interrupt void Timer1_A0(void)
{

  TA0CCR1 += IncDec_PWM * 2;
  if (TA0CCR1 > 998 || TA0CCR1 < 2)
    IncDec_PWM = -IncDec_PWM;
}