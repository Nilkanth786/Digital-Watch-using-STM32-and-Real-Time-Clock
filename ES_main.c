#include "stm32f10x.h"   
#include "I2C.h"
#include "SysTick_Timer.h"

char data[] = {"Hello World"};
int main(void)
{
	systick_init();
	i2c_init(2,i2c_SM);
	while(1)
	{
		i2c_write(2, 0x78,data);
		DelayMs(10);
	}
}
