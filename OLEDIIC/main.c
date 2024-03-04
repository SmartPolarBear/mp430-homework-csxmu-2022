#include "msp430g2553.h"
#include"I2C_OLED.H"
#include"zimo.h"

#define LEN(arr) (sizeof(arr)/sizeof(arr[0]))

int name_indexes[]={1,2,7,8,9};
int major_indexes[]={5,6,10,11,12,13,14};
int sid[]={2,2,9,2,0,2,0,2,2,0,4,6,2,2};

int main(void)
{
   system_clock();
   I2C_OLED_Init();
   while(1)
   {
	   OLED_All(0);
//	   delay_ms(500);


	   volatile int i=0,cnt=0;
	   for(i=0;i<LEN(name_indexes);i++)
	   {
	       OLED_P16x16Ch((cnt++)*16,0,name_indexes[i]);
	   }

	   i=cnt=0;
	   for(i=0;i<LEN(major_indexes);i++)
	   {
	       OLED_P16x16Ch((cnt++)*16,3,major_indexes[i]);
	   }

	   OLED_P16x16Ch(0,6,3);
	   OLED_P16x16Ch(16,6,4);
	   OLED_P6x8Str(32,6,"22920202204622");

	   for(;;);
   }
}
