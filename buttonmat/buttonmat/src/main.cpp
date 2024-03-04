// #include <msp430g2553.h>

// void set_output(int pn, int kth)
// {
//   unsigned char val = 1u << kth;
//   if (pn == 1)
//   {
//     P1DIR |= val;
//     P1OUT &= ~val;
//   }
//   else if (pn == 2)
//   {
//     P2DIR |= val;
//     P2OUT &= ~val;
//   }
// }

// void set_input(int pn, int kth)
// {
//   unsigned char val = 1u << kth;
//   if (pn == 1)
//   {
//     P1DIR &= ~val;
//     P1OUT |= val;
//     P1REN |= val;
//   }
//   else if (pn == 2)
//   {
//     P2DIR &= ~val;
//     P2OUT |= val;
//     P2REN |= val;
//   }
// }

// void sti(int pn, int k)
// {
//   unsigned char val = 1u << k;
//   if (pn == 1)
//   {
//     P1IES |= val;
//     P1IFG &= ~val;
//     P1IE |= val;
//   }
//   else if (pn == 2)
//   {
//     P2IES |= val;
//     P2IFG &= ~val;
//     P2IE |= val;
//   }
// }

// void cli(int pn, int k)
// {
//   unsigned char val = 1u << k;
//   if (pn == 1)
//   {
//     P1IFG &= ~val;
//   }
//   else if (pn == 2)
//   {
//     P2IFG &= ~val;
//   }
// }

// void __attribute__((interrupt(PORT1_VECTOR))) button1_isr(void)
// {
//   if (P1IFG & BIT3)
//   {
//     P1OUT ^= BIT0;
//     cli(1, 3);
//   }
//   else if (P1IFG & BIT4)
//   {
//     P1OUT ^= (BIT0 | BIT6);
//     cli(1, 4);
//   }
//   else if (P1IFG & BIT5)
//   {
//     P1OUT ^= BIT6;
//     cli(1, 5);
//   }
// }

// int main()
// {
//   WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

//   set_output(1, 6);
//   set_output(1, 0);

//   for (volatile int i = 2; i <= 5; i++)
//   {
//     set_input(1, i);
//     sti(1, i);
//   }

//   __bis_SR_register(GIE);
//   return 0;
// }

#include <msp430.h> 


/**
 * main.c
 */

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(PORT1_VECTOR))) Port_1(void)
#else
#error Compiler not supported!
#endif
{
    if(!(P1IN & BIT4))
    {
        P1OUT ^= BIT0;
        P1IFG &= ~BIT4;
    }
    if(!(P1IN & BIT5))
    {
        P1OUT ^= BIT6;
        P1IFG &= ~BIT5;
    }

}

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P1DIR |= BIT6;
    P1OUT &= ~BIT6;

    P1DIR &= ~BIT4;
    P1OUT != BIT4;
    P1REN |= BIT4;

    P1DIR &= ~BIT5;
    P1OUT != BIT5;
    P1REN |= BIT5;

    P1IES |= BIT4;
    P1IFG &= ~BIT4;
    P1IE |= BIT4;

    P1IES |= BIT5;
    P1IFG &= ~BIT5;
    P1IE |= BIT5;

    __bis_SR_register(GIE);
    return 0;
}

