#include "common.h"
#include "stdlib.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI STA驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 netpro=0;	//网络模式
u8 atk_8266_wifista_test(void)
{
	
	u8 *p;
		p=mymalloc(SRAMIN,32);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50); //设置WIFI STA模式
	atk_8266_send_cmd("AT+RST","OK",20);	  //DHCP服务器关闭(仅AP模式有效) 
	delay_ms(1000);         //延时3S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
	printf("正在连接服务器,请稍等...\r\n");
	delay_ms(300);
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);//0：单连接，1：多连接
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"TCP.lewei50.com\",%s",(u8*)portnum);
	while(atk_8266_send_cmd(p,"OK",200))
	{
		printf("连接TCP服务器失败\r\n");//连接失败
	}	
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传			
	delay_ms(10);
	atk_8266_quit_trans();
	atk_8266_send_cmd("AT+CIPSEND","OK",20);
	myfree(SRAMIN,p);		//释放内存 
	return 1;
} 
