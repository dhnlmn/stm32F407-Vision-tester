#include "common.h"
#include "stdlib.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI STA��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 netpro=0;	//����ģʽ
u8 atk_8266_wifista_test(void)
{
	
	u8 *p;
		p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50); //����WIFI STAģʽ
	atk_8266_send_cmd("AT+RST","OK",20);	  //DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
	printf("�������ӷ�����,���Ե�...\r\n");
	delay_ms(300);
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);//0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"TCP.lewei50.com\",%s",(u8*)portnum);
	while(atk_8266_send_cmd(p,"OK",200))
	{
		printf("����TCP������ʧ��\r\n");//����ʧ��
	}	
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��			
	delay_ms(10);
	atk_8266_quit_trans();
	atk_8266_send_cmd("AT+CIPSEND","OK",20);
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	return 1;
} 
