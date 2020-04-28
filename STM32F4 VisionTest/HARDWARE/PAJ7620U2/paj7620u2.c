#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 探索者STM32F407开发板
//PAJ7620U2 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2017/7/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//选择PAJ7620U2 BANK区域
u16 gesture_data;
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0寄存器区域
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1寄存器区域
	}
			
}

//PAJ7620U2唤醒
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	GS_WakeUp();//唤醒PAJ7620U2
	delay_ms(5);//唤醒时间>400us
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	data = GS_Read_Byte(0x00);//读取状态
	if(data!=0x20) return 0; //唤醒失败
	
	return 1;
}

//PAJ7620U2初始化
//返回值：0:失败 1:成功
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC初始化
    status = paj7620u2_wakeup();//唤醒PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//初始化PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	return 1;
}

//主菜单
void paj7620u2_test_ui(void)
{
	POINT_COLOR=BLUE;//设置字体为蓝色
	//LCD_Fill(30,170,300,300,WHITE);
	//LCD_ShowString(30,170,200,16,16,"KEY1:   Gesture test");//手势识别测试
	//LCD_ShowString(30,190,200,16,16,"KEY0:   Ps test     ");//接近检测测试
	
}

//手势识别测试
void Gesture_test(void)
{
	u8 i;
    u8 status;
	u8 key;
	u8 data[2]={0x00};
	
	u8 ledflash=0;
	
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	i=0;
	POINT_COLOR=BLUE;//设置字体为蓝色
	//LCD_Fill(30,170,300,300,WHITE);
	//LCD_ShowString(30,180,200,16,16,"KEY_UP: Exit the test");
	//LCD_ShowString(30,210,200,16,16,"Gesture test");
	POINT_COLOR=RED;//设置字体为蓝色
	
        key = KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//关闭手势识别中断输出
			GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
			
		}			
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态			
		if(!status)
		{   
			gesture_data =(u16)data[1]<<8 | data[0];
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:               GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
					                                       ledflash=1;      break; //向上
					case GES_DOWM:                  GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
               						                    ledflash=1;      break; //向下
					case GES_LEFT:                       GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
  						                                ledflash=1;      break; //向左
					case GES_RIGHT:                 GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
                						               ledflash=1;      break; //向右
					case GES_FORWARD:                  
						                             ledflash=1;      break; //向前
					case GES_BACKWARD:            
            						                 ledflash=1;      break; //向后
					case GES_CLOCKWISE:           
                						           ledflash=1;      break; //顺时针
					case GES_COUNT_CLOCKWISE:   
                   						      ledflash=1;      break; //逆时针
					case GES_WAVE:                
						                                ledflash=1;      break; //挥动
					default:  ledflash=0; break;
					
				}	
                if(ledflash)//DS1闪烁
				{   
					LED1=0;delay_ms(80);LED1=1;delay_ms(80);
					LED1=0;delay_ms(80);LED1=1;delay_ms(80);
					delay_ms(300);
					//LCD_ShowString(40,250,200,16,24,"                        ");
					ledflash=0;
				}						
			}
			
		}
		delay_ms(50);
		i++;
		if(i==5)
		{
			LED0=!LED0;//提示系统正在运行	
			i=0;
		}		   
	}


//接近检测测试
void Ps_test(void)
{
	u8 i;
	u8 key;
	u8 data[2]={0x00};
	u8 obj_brightness=0;
	u16 obj_size=0;
	
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//接近检测模式初始化
	}
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	i=0;
	/*POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_Fill(30,170,300,300,WHITE);
	LCD_ShowString(30,180,200,16,16,"KEY_UP: Exit the test");
	LCD_ShowString(30,210,200,16,16,"Ps test");
	LCD_ShowString(30,240,200,16,16,"Brightness");
	LCD_ShowString(160,240,200,16,16,"Size");
	POINT_COLOR=RED;//设置字体为蓝色	*/
	
	while(1)
	{	
		key = KEY_Scan(0);
		if(key==WKUP_PRES) break;
		
		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//读取物体亮度
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//读取物体大小
		data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
		obj_size = ((u16)data[1] & 0x0f)<<8 | data[0];
		LCD_ShowxNum(50,270,obj_brightness,3,24,0);
		LCD_ShowxNum(152,270,obj_size,3,24,0);
		printf("obj_brightness: %d\r\n",obj_brightness);
        printf("obj_size: %d\r\n",obj_size);
		
		delay_ms(100);
		i++;
		if(i==5)
		{
		    LED0=!LED0;//提示系统正在运行	
			i=0;
		}
	}
	
}
//PAJ7620U2传感器测试
void paj7620u2_sensor_test(void)
{   
	 u8 i=0;
	 u8 key;
	
     paj7620u2_test_ui();//主菜单显示
	 while(1)
	 {
		 key = KEY_Scan(0);//按键扫描
		 if(key)
		 {
			 switch(key)
			 {
				 case KEY1_PRES:  Gesture_test();   break;//手势检测模式
				 case KEY0_PRES:  Ps_test();        break;//接近检测模式  
			 }
			 paj7620u2_test_ui();
		 }
		delay_ms(50);
		i++;
		if(i==5)
		{
		    LED0=!LED0;//提示系统正在运行	
			i=0;
		}
		 
	 }
}
