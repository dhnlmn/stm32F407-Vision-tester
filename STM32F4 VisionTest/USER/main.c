#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "JQ8X00.h"
#include "audio.h"
#include "lcd.h"
#include "key.h"
#include "paj7620u2.h"
#include "24cxx.h"
#include "vl53l0x.h"
#include "vl53l0x_gen.h"
#include "common.h"
#include "timer.h"
#include "usart3.h"
int main(void)
{
	int t,d; 
	int size[7]={12,16,24,32,48,96,128};
	int correct=0;
	int error=0;
	int flat=0;
	u8 AUDIO_BUF1[6] = {0xAA, 0x07, 0x02, 0x00, 0x01, 0xB4};
	u8 AUDIO_BUF2[6] = {0xAA, 0x07, 0x02, 0x00, 0x02, 0xB5};
	u8 AUDIO_BUF3[6] = {0xAA, 0x07, 0x02, 0x00, 0x03, 0xB6};
	u8 AUDIO_BUF4[6] = {0xAA, 0x07, 0x02, 0x00, 0x05, 0xB8};
	u8 AUDIO_BUF5[6] = {0xAA, 0x07, 0x02, 0x00, 0x04, 0xB7};  //音量播放选择
	u8 AUDIO_BUF6[6] = {0xAA, 0x07, 0x02, 0x00, 0x0A, 0xBD};  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(9600);	//串口初始化波特率为115200
	usart2_init(115200);
	AUDIO_VOL();//设置音量20
	AUDIO_PLAY(AUDIO_BUF3);
	atk_8266_test();		//进入ATK_ESP8266测试
	
	//u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");//云登陆
	//u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0\"}]}&^!\r\n");
	AUDIO_PLAY(AUDIO_BUF5);
  AT24CXX_Init();		//IIC初始化 
	KEY_Init();
	LCD_Init();
	LED_Init();	
	paj7620u2_init();
  delay_ms(100);
	//OneLine_ZHControl(30,Volume);
 
	POINT_COLOR=RED;

	d=4;
	LCD_ShowString(120,30,300,300,size[d],"E");
	LED0=0;
	while(1)
	{
			vl53l0x_test();
	if(Distance_data<=300||Distance_data>=400)
	{
			 LED0=0;
	
	}
	else{AUDIO_PLAY(AUDIO_BUF6);
			while(1)
	{
			LED1=0;
		
		 LED0=1;
			 //LED1=0;
		Gesture_test();
		t=KEY_Scan(0);
		if(gesture_data==GES_RIGHT)
		{
			if(i==0|i==1)
			{
				i=rand()%8;
				LCD_Init();
				correct++;
				AUDIO_PLAY(AUDIO_BUF1);
			if(correct==1)
		  {
			
				LCD_ShowString(110,110,300,300,size[d],"E");
		  }
		else if(correct==2)
		{
			d--;
			LCD_ShowString(110,110,300,300,size[d],"E");
			correct=0;
		}
			
		}//播放内存中第一条语音
		else {
			i=rand()%8;LCD_Init();
			AUDIO_PLAY(AUDIO_BUF2);
				error++;
			flat++;
			correct=0;
			
			if(error==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(error==2)
		{
		d++;
			LCD_ShowString(110,110,300,300,size[d],"E");
			error=0;
		}
		}
	}
		else if(gesture_data==GES_DOWM)
		{
			if(i==4|i==5)
			{
				i=rand()%8;
			AUDIO_PLAY(AUDIO_BUF1);
				correct++;
				LCD_Init();
			if(correct==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(correct==2)
		{
			d--;
			LCD_ShowString(110,110,300,300,size[d],"E");
			correct=0;
		}
			
		}//播放内存中第一条语音
			else {
				i=rand()%8;LCD_Init();
				AUDIO_PLAY(AUDIO_BUF2);
				error++;
				flat++;
			correct=0;
				
		if(error==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(error==2)
		{
			d++;
			LCD_ShowString(110,110,300,300,size[d],"E");
			error=0;
		}
	}
}
		else if(gesture_data==GES_LEFT)
		{
			if(i==2|i==3)
			{
				i=rand()%8;
			LCD_Init();
				correct++;
				AUDIO_PLAY(AUDIO_BUF1);
			if(correct==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(correct==2)
		{
			d--;
			LCD_ShowString(110,110,300,300,size[d],"E");
			correct=0;
		}
			
		}//播放内存中第一条语音
	else {
		i=rand()%8;LCD_Init();
		AUDIO_PLAY(AUDIO_BUF2);
			error++;
			flat++;
			correct=0;
		
		
			if(error==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(error==2)
		{
		d++;
			LCD_ShowString(110,110,300,300,size[d],"E");
			error=0;
		}
		}
		}
	
		
		else if(gesture_data==GES_UP)
		{
			if(i==6|i==7)
			{i=rand()%8;
			LCD_Init();
				correct++;
				AUDIO_PLAY(AUDIO_BUF1);
			if(correct==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(correct==2)
		{
			d--;
			LCD_ShowString(110,110,300,300,size[d],"E");
			correct=0;
		}
			
		}//播放内存中第一条语音
			else {
				i=rand()%8;
				LCD_Init();
				AUDIO_PLAY(AUDIO_BUF2);
				error++;
				flat++;
			correct=0;
				
			if(error==1)
		{
			
			LCD_ShowString(110,110,300,300,size[d],"E");
		}
		else if(error==2)
		{
		d++;
			LCD_ShowString(110,110,300,300,size[d],"E");
			error=0;
		}
		}
		
		
	}
		else if(flat==4|(d==0&&correct==1)|(d==6&&error==1))
		{flat=0;
			correct=0;
//			error=0;'
			i=0;
			AUDIO_PLAY(AUDIO_BUF4);
			LCD_Init();
			
			if(d==0) //测试结果
			{
				LCD_ShowString(110,110,300,300,24,"1.0");
				u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"1.0\"}]}&^!\r\n");
			}
			else if(d==1)
			{
				LCD_ShowString(110,110,300,300,24,"0.9");
				
				u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0.9\"}]}&^!\r\n");
			}
			else if(d==2)
			{
				LCD_ShowString(110,110,300,300,24,"0.8");
				u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0.8\"}]}&^!\r\n");
			}
			else if(d==3)
			{
				LCD_ShowString(110,110,300,300,24,"0.7");
				u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0.7\"}]}&^!\r\n");
			}
			else if(d==4)
			{
				LCD_ShowString(110,110,300,300,24,"0.6");
				u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0.6\"}]}&^!\r\n");
			}
			else if(d==5)
			{
				LCD_ShowString(110,110,300,300,24,"0.5");
				u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0.5\"}]}&^!\r\n");
			}
			else if(d==6)
			{
				LCD_ShowString(110,110,300,300,24,"0.2");	
u2_printf("{\"method\": \"update\",\"gatewayNo\": \"01\",\"userkey\": \"95276e9a67ea4c8ea486ec2bee74713c\"}&^!\r\n");				
					u2_printf("{\"method\": \"upload\",\"data\":[{\"Name\":\"T1\",\"Value\":\"0.2\"}]}&^!\r\n");
			}
			while(1)
			{
			}
		}
}	 
 }
	}		

	
}
