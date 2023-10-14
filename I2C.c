#include "I2C.h"
/* 
*************************Author-Nilkanth Kapadia & Harshesh Shah**********************************
Psuedo Code:
1.Make Initialization Function 
2.Set Start Bit
3.Clear reading SR1 register followed by writing DR register with Address.
4.ADDR=1, cleared by reading SR1 register followed by reading SR2.
*/
/*
I2C2
PB10 -> SCL
PB11 -> SDA
I2C1
PB6 -> SCL
PB7 -> SDA
*/
// i2c_init()
// Start step
// Sending the address + R or Write	
// Sending data step
// Stop step

void i2c_init(char i2c,unsigned short speed_mode)
{
	RCC->APB2ENR |= 1;   //Enable the Alternate Function Mode
	if(i2c==1)
	{
		RCC->APB1ENR |= 0x200000;   //Enable the I2C peripheral-1
		// Pin enable 
		init_GP(PB,6,OUT50,O_AF_OD);  // Select Pin-6 
		init_GP(PB,7,OUT50,O_AF_OD);  // Select Pin-7
		I2C1->CR1 |= 0x8000;          // Software reset when we activate the I2C peripheral
		I2C1->CR1 &= ~0x8000;         
		I2C1->CR2 =0x8;
		I2C1->CCR = speed_mode;
		I2C1->TRISE = 0x9;
		I2C1->CR1 |= 1;
	}
	else if(i2c==2)
	{
		RCC->APB1ENR |= 0x400000;
		// Pin enable 
		init_GP(PB,10,OUT50,O_AF_OD);
		init_GP(PB,11,OUT50,O_AF_OD);
		I2C2->CR1 |= 0x8000;
		I2C2->CR1 &= ~0x8000;
		I2C2->CR2 =0x8;
		I2C2->CCR = speed_mode;
		I2C2->TRISE = 0x9;
		I2C2->CR1 |= 1;
	}
}

// Start step
void i2c_start(char i2c)
{
	if(i2c==1)
	{
		I2C1->CR1 |= 0x100;
		while (!(I2C1->SR1 & 1)){};
	}
	else if(i2c==2)
	{
		I2C2->CR1 |= 0x100;
		while (!(I2C2->SR1 & 1)){};
	}
}
// Sending the address + R or Write	
char i2c_add(char i2c, char address,char RW)
{
	volatile int tmp;
	if(i2c==1)
	{
		I2C1->DR = (address|RW);
		while((I2C1->SR1 & 2)==0){};
		while((I2C1->SR1 & 2)){
			tmp = I2C1->SR1;
			tmp = I2C1->SR2;
			if((I2C1->SR1 & 2)==0)
			{
				break;
			}
		}
	}
	else if(i2c==2)
	{
		I2C2->DR = (address|RW);
		while((I2C2->SR1 & 2)==0){};
		while((I2C2->SR1 & 2)){
			tmp = I2C2->SR1;
			tmp = I2C2->SR2;
			if((I2C2->SR1 & 2)==0)
			{
				break;
			}
		}
	}

}
// Sending data step
void i2c_data(char i2c,char data)
{
	if(i2c==1)
	{
		while((I2C1->SR1 & 0x80) == 0){}
			I2C1->DR = data;
		while((I2C1->SR1 & 0x80) == 0){}
	}
	else if(i2c==2)
	{
		while((I2C2->SR1 & 0x80) == 0){}
			I2C2->DR = data;
		while((I2C2->SR1 & 0x80) == 0){}
	}
}

// Stop step
void i2c_stop(char i2c)
{
	volatile int tmp;
	if(i2c==1)
	{
		tmp = I2C1->SR1;
		tmp = I2C1->SR2;
		I2C1->CR1 |= 0x200;
	}
	else if(i2c==2)
	{
		tmp = I2C2->SR1;
		tmp = I2C2->SR2;
		I2C2->CR1 |= 0x200;
	}
}
// i2c_write()
void i2c_write(char i2c, char address,char data[])
{
	int i = 0;
	
	i2c_start(i2c);
	
	i2c_add(i2c, address,0);
	
	while(data[i])
		{
			i2c_data(i2c,data[i]);
			i++;
		}
	i2c_stop(i2c);
}
// Receiving data
char i2c_rx(char i2c, char ACK_NACK)
{
	char temp;
	if(i2c==1)
	{
		I2C1->CR1 |= 0x0400;
		while((I2C1->SR1 & 0x40)==0){}
			temp = I2C1->DR;
		if(ACK_NACK)
		{
			I2C1->CR1 &= ~0x0400;
		}
		
	}
	else if(i2c==2)
	{
		I2C2->CR1 |= 0x0400;
		while((I2C2->SR1 & 0x40)==0){}
			temp = I2C2->DR;
		if(ACK_NACK)
		{
			I2C2->CR1 &= ~0x0400;
		}
		
	}
	return temp;
}
char i2c_check(char i2c)
{
	char check=1;
	if(i2c ==1)
	{
		if(I2C1->SR2 & 2)
		{
			check =0;
		}
	}
	else if(i2c ==2)
	{
		if(I2C2->SR2 & 2)
		{
			check =0;
		}
	}
	return check;
}
