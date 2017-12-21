#include"DELAY.H"
#include "intrins.h"
void Delay1(unsigned long ms)//简单延迟函数，单位是毫秒
{
	unsigned char i, j,k;
	for(k=0;k<ms;k++)
	{
		_nop_();
		_nop_();
		i = 22;
		j = 128;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Delay2(unsigned long cnt)
{
	long i;
 	for(i=0;i<cnt*100;i++);
}
