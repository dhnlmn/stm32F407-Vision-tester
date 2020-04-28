#include "paj7620u2_iic.h"
#include "paj7620u2.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 探索者STM32F407开发板
//PAJ7620U2 IIC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2017/7/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//PAJ2670 I2C初始化
void GS_i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化

	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);//PB10,PB11 输出高	
	
}

//产生IIC起始信号
static void GS_IIC_Start(void)
{
	GS_SDA_OUT();//sda线输出
	GS_IIC_SDA=1;	  	  
	GS_IIC_SCL=1;
	delay_us(4);
 	GS_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	GS_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
static void GS_IIC_Stop(void)
{
	GS_SDA_OUT();//sda线输出
	GS_IIC_SCL=0;
	GS_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GS_IIC_SCL=1; 
	GS_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static u8 GS_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	GS_SDA_IN();  //SDA设置为输入  
	GS_IIC_SDA=1;delay_us(3);	   
	GS_IIC_SCL=1;delay_us(3);	 
	while(GS_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GS_IIC_Stop();
			return 1;
		}
	}
	GS_IIC_SCL=0;//时钟输出0 	   
	return 0;  
}

//产生ACK应答
static void GS_IIC_Ack(void)
{
	GS_IIC_SCL=0;
	GS_SDA_OUT();
	GS_IIC_SDA=0;
	delay_us(3);
	GS_IIC_SCL=1;
	delay_us(3);
	GS_IIC_SCL=0;
}

//不产生ACK应答		    
static void GS_IIC_NAck(void)
{
	GS_IIC_SCL=0;
	GS_SDA_OUT();
	GS_IIC_SDA=1;
	delay_us(2);
	GS_IIC_SCL=1;
	delay_us(2);
	GS_IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
static void GS_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	GS_SDA_OUT(); 	    
    GS_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			GS_IIC_SDA=1;
		else
			GS_IIC_SDA=0;
		txd<<=1; 	  
		delay_us(5);  
		GS_IIC_SCL=1;
		delay_us(5); 
		GS_IIC_SCL=0;	
		delay_us(5);
    }	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
static u8 GS_IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	GS_SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		GS_IIC_SCL=0; 
		delay_us(4);
	  GS_IIC_SCL=1;
		receive<<=1;
		if(GS_READ_SDA)receive++;   
	  delay_us(4); 
	}					 
	if (!ack)
		GS_IIC_NAck();//发送nACK
	else
		GS_IIC_Ack(); //发送ACK   
	return receive;
}

//PAJ7620U2写一个字节数据
u8 GS_Write_Byte(u8 REG_Address,u8 REG_data)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);
	if(GS_IIC_Wait_Ack())
	{
		GS_IIC_Stop();//释放总线
		return 1;//没应答则退出

	}
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();	
	GS_IIC_Send_Byte(REG_data);
	GS_IIC_Wait_Ack();	
	GS_IIC_Stop();

	return 0;
}

//PAJ7620U2读一个字节数据
u8 GS_Read_Byte(u8 REG_Address)
{
	u8 REG_data;
	
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//发写命令
	if(GS_IIC_Wait_Ack())
	{
		 GS_IIC_Stop();//释放总线
		 return 0;//没应答则退出
	}		
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();
	GS_IIC_Start(); 
	GS_IIC_Send_Byte(PAJ7620_ID|0x01);//发读命令
	GS_IIC_Wait_Ack();
	REG_data = GS_IIC_Read_Byte(0);
	GS_IIC_Stop();

	return REG_data;
}
//PAJ7620U2读n个字节数据
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//发写命令
	if(GS_IIC_Wait_Ack()) 
	{
		GS_IIC_Stop();//释放总线
		return 1;//没应答则退出
	}
	GS_IIC_Send_Byte(REG_Address);
	GS_IIC_Wait_Ack();

	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID|0x01);//发读命令
	GS_IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf = GS_IIC_Read_Byte(0);
		}
		else
		{
			*buf = GS_IIC_Read_Byte(1);
		}
		buf++;
		len--;
	}
	GS_IIC_Stop();//释放总线

	return 0;
	
}
//PAJ7620唤醒
void GS_WakeUp(void)
{
	GS_IIC_Start();
	GS_IIC_Send_Byte(PAJ7620_ID);//发写命令
	GS_IIC_Stop();//释放总线
}


