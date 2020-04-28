#include "audio.h"
#include "usart.h"
#include "delay.h"
void AUDIO_PLAY(u8 AUDIO_BUF[6])
{
	u16 t; 
	for(t=0;t<6;t++)
	{
			USART_SendData(USART1, AUDIO_BUF[t]);//向串口1发送数据
			delay_ms(200);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}	
	USART_RX_STA=0;	
	//delay_ms(20000);
}
void AUDIO_VOL(void)
{
	u16 t;
	u8 AUDIO_VOL[15] = {0xAA, 0x13, 0x01, 0x1E, 0xDC};
	for(t=0;t<6;t++)
	{
			USART_SendData(USART1, AUDIO_VOL[t]);//向串口1发送数据
			delay_ms(200);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
	}	
	USART_RX_STA=0;	
	//delay_ms(20000);
	
}

