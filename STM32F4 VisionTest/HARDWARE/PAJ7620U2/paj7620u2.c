#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ̽����STM32F407������
//PAJ7620U2 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//ѡ��PAJ7620U2 BANK����
u16 gesture_data;
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
	}
			
}

//PAJ7620U2����
u8 paj7620u2_wakeup(void)
{ 
	u8 data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) return 0; //����ʧ��
	
	return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
u8 paj7620u2_init(void)
{
	u8 i;
	u8 status;
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	
	return 1;
}

//���˵�
void paj7620u2_test_ui(void)
{
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	//LCD_Fill(30,170,300,300,WHITE);
	//LCD_ShowString(30,170,200,16,16,"KEY1:   Gesture test");//����ʶ�����
	//LCD_ShowString(30,190,200,16,16,"KEY0:   Ps test     ");//�ӽ�������
	
}

//����ʶ�����
void Gesture_test(void)
{
	u8 i;
    u8 status;
	u8 key;
	u8 data[2]={0x00};
	
	u8 ledflash=0;
	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	i=0;
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	//LCD_Fill(30,170,300,300,WHITE);
	//LCD_ShowString(30,180,200,16,16,"KEY_UP: Exit the test");
	//LCD_ShowString(30,210,200,16,16,"Gesture test");
	POINT_COLOR=RED;//��������Ϊ��ɫ
	
        key = KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);//�ر�����ʶ���ж����
			GS_Write_Byte(PAJ_SET_INT_FLAG2,0X00);
			
		}			
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//��ȡ����״̬			
		if(!status)
		{   
			gesture_data =(u16)data[1]<<8 | data[0];
			if(gesture_data) 
			{
				switch(gesture_data)
				{
					case GES_UP:               GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
					                                       ledflash=1;      break; //����
					case GES_DOWM:                  GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
               						                    ledflash=1;      break; //����
					case GES_LEFT:                       GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
  						                                ledflash=1;      break; //����
					case GES_RIGHT:                 GS_Write_Byte(PAJ_SET_INT_FLAG1,0X00);
                						               ledflash=1;      break; //����
					case GES_FORWARD:                  
						                             ledflash=1;      break; //��ǰ
					case GES_BACKWARD:            
            						                 ledflash=1;      break; //���
					case GES_CLOCKWISE:           
                						           ledflash=1;      break; //˳ʱ��
					case GES_COUNT_CLOCKWISE:   
                   						      ledflash=1;      break; //��ʱ��
					case GES_WAVE:                
						                                ledflash=1;      break; //�Ӷ�
					default:  ledflash=0; break;
					
				}	
                if(ledflash)//DS1��˸
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
			LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}		   
	}


//�ӽ�������
void Ps_test(void)
{
	u8 i;
	u8 key;
	u8 data[2]={0x00};
	u8 obj_brightness=0;
	u16 obj_size=0;
	
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<PROXIM_SIZE;i++)
	{
		GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//�ӽ����ģʽ��ʼ��
	}
	paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
	i=0;
	/*POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_Fill(30,170,300,300,WHITE);
	LCD_ShowString(30,180,200,16,16,"KEY_UP: Exit the test");
	LCD_ShowString(30,210,200,16,16,"Ps test");
	LCD_ShowString(30,240,200,16,16,"Brightness");
	LCD_ShowString(160,240,200,16,16,"Size");
	POINT_COLOR=RED;//��������Ϊ��ɫ	*/
	
	while(1)
	{	
		key = KEY_Scan(0);
		if(key==WKUP_PRES) break;
		
		obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//��ȡ��������
		data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//��ȡ�����С
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
		    LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}
	}
	
}
//PAJ7620U2����������
void paj7620u2_sensor_test(void)
{   
	 u8 i=0;
	 u8 key;
	
     paj7620u2_test_ui();//���˵���ʾ
	 while(1)
	 {
		 key = KEY_Scan(0);//����ɨ��
		 if(key)
		 {
			 switch(key)
			 {
				 case KEY1_PRES:  Gesture_test();   break;//���Ƽ��ģʽ
				 case KEY0_PRES:  Ps_test();        break;//�ӽ����ģʽ  
			 }
			 paj7620u2_test_ui();
		 }
		delay_ms(50);
		i++;
		if(i==5)
		{
		    LED0=!LED0;//��ʾϵͳ��������	
			i=0;
		}
		 
	 }
}
