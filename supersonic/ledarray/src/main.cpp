#include <msp430g2553.h>
constexpr auto TRIG_PIN = BIT1; // Corresponds to P2.1
constexpr auto ECHO_PIN = BIT1; // Corresponds to P1.1

constexpr auto RLED = BIT5; // P2.5
constexpr auto GLED = BIT6; // P1.6
constexpr auto BLED = BIT7; // P1.7

constexpr auto BEEP = BIT4; // P2.4

volatile unsigned long start_time;

volatile unsigned long end_time;

volatile unsigned long delta_time;

volatile unsigned long long distance;

void wait_ms(unsigned int ms)
{
	for (unsigned int i = 0; i < ms; i++)
	{

		__delay_cycles(1000);
	}
}

void turn_off_all()
{
	P1OUT &= ~BIT0;
	P1OUT &= ~GLED;
	P1OUT &= ~BLED;
	P2OUT &= ~RLED;
}

void update_led()
{
	turn_off_all();
	if (distance > 1 && distance <= 10)
	{
		P1OUT |= BIT0;
		P2OUT |= RLED; // red
		P2OUT |= BEEP;
	}
	else if (distance > 10 && distance <= 25)
	{
		P1OUT |= BLED;
		P2OUT &= ~BEEP;
	}
	else if (distance > 25)
	{
		P1OUT |= GLED;
		P2OUT &= ~BEEP;
	}
}

void __attribute__((interrupt(TIMER0_A0_VECTOR))) ta1_isr(void)
{

	switch (TAIV)
	{
	// Timer overflow
	case 10:
		break;

		// Otherwise Capture Interrupt
	default:
		// Read the CCI bit (ECHO signal) in CCTL0

		// If ECHO is HIGH then start counting (rising edge)

		if (CCTL0 & CCI)
		{
			start_time = CCR0;

		} // If ECHO is LOW then stop counting (falling edge)
		else
		{
			end_time = CCR0;

			delta_time = end_time - start_time;

			distance = (unsigned long)(delta_time / 58.3090379);
		}
		break;
	}
	TACTL &= ~CCIFG; // reset the interrupt flag
}

void reset_timer(void)
{
	// Clear timer
	TACTL |= TACLR;
}

int main()
{
	// Stop Watch Dog Timer
	WDTCTL = WDTPW + WDTHOLD;

	// Set ECHO (P1.1) pin as INPUT
	P1DIR &= ~ECHO_PIN;

	// Set P1.1 as CCI0A (Capture Input signal).
	P1SEL |= ECHO_PIN;

	// Set TRIGGER (P2.1) and RLED pin as OUTPUT
	P2DIR |= TRIG_PIN;
	P2DIR |= RLED;
	P2DIR |= BEEP;

	// Set TRIGGER (P2.1) pin to LOW
	P2OUT &= ~TRIG_PIN;
	P2OUT &= ~RLED;
	P2OUT &= ~BEEP;

	P1DIR |= BIT0;
	P1DIR |= GLED;
	P1DIR |= BLED;

	P1OUT &= ~BIT0;
	P1OUT &= ~GLED;
	P1OUT &= ~BLED;

	/* Use internal calibrated 1MHz clock: */
	BCSCTL1 = CALBC1_1MHZ; // Set range

	DCOCTL = CALDCO_1MHZ;

	BCSCTL2 &= ~(DIVS_3); // SMCLK = DCO = 1MHz

	// Stop timer before modifying the configuration
	TACTL = MC_0;

	CCTL0 |= CM_3 + SCS + CCIS_0 + CAP + CCIE;

	// Select SMCLK with no divisions, continuous mode.
	TACTL |= TASSEL_2 + MC_2 + ID_0;

	// Global Interrupt Enable
	__enable_interrupt();

	for (;;)
	{

		// send ultrasonic pulse
		reset_timer();

		// Enable TRIGGER
		P2OUT |= TRIG_PIN;

		// Send pulse for 10us
		__delay_cycles(10);

		// Disable TRIGGER
		P2OUT &= ~TRIG_PIN;

		update_led();

		// wait 500ms until next measurement
		wait_ms(200);
	}
	return 0;
}
