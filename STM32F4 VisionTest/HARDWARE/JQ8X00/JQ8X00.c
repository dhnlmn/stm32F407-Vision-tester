#include "JQ8X00.h"
#include "usart.h"	 
#include "delay.h"

#define ZH_MAX	50		//组合播放最大容量

/************************************************************************
功能描述：模块IO初始化函数
入口参数： 无
返 回 值： none
其他说明： 如果采用单线控制JQ8x00_Workmode的值改为1
**************************************************************************/
void JQ_8x00_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(JQ8x00_BUSY_RCC | JQ8x00_VPP_RCC,ENABLE);	 //使能时钟
	
	GPIO_InitStructure.GPIO_Pin  = JQ8x00_BUSY_GPIO_Pin;		//JQ8X00忙检测
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
 	GPIO_Init(JQ8x00_BUSY_GPIO, &GPIO_InitStructure);
	
	#if JQ8x00_Workmode
	GPIO_InitStructure.GPIO_Pin  = JQ8x00_VPP_GPIO_Pin;						//JQ8x00单线控制IO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(JQ8x00_VPP_GPIO, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1
	GPIO_SetBits(JQ8x00_VPP_GPIO,JQ8x00_VPP_GPIO_Pin);						 	//输出高
	#endif
}

/************************条件编译**************************************/
#if JQ8x00_Workmode						//控制方式选择

/******************************单线控制********************************/

/************************************************************************
功能描述：	单线控制单字节发送函数
入口参数： 	DATA：发送数据
返 回 值： none
其他说明： 将需要发送的数据作为形参传入
**************************************************************************/
void OneLine_SendData(u8 DATA)
{
	u8 i;
	
	JQ8x00_VPP_Set();		//开始信号
	delay_us(100);
	JQ8x00_VPP_Clr();
	delay_ms(3);
	
	for(i=0;i<8;i++)
	{
		JQ8x00_VPP_Set();
		if(DATA & 0x01)
		{
			delay_us(1500);
			JQ8x00_VPP_Clr();
			delay_us(500);
		}
		else
		{
			delay_us(500);
			JQ8x00_VPP_Clr();
			delay_us(1500);
		}
		DATA >>= 1;
	} 
	
	JQ8x00_VPP_Set();
}

/************************************************************************
功能描述：	单线字节控制
入口参数： 	Mode：功能
返 回 值： none
其他说明： 将需要的功能作为形参传入
**************************************************************************/
void OneLine_ByteControl(LineByteSelect Mode)
{
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//忙检测
	#endif
	OneLine_SendData(Mode);
}

/************************************************************************
功能描述：	单线控制组合发送函数
入口参数： 	Nume：数字，Mode：功能
返 回 值： none
其他说明： 将需要发送的数据和需要的功能作为形参传入
**************************************************************************/
void OneLine_ZHControl(u8 Nume,LineModeSelect Mode)
{
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//忙检测
	#endif
	OneLine_SendData(0x0a);
	if(Nume < 10)
	{
		OneLine_SendData(Nume);
	}
	else
	{
		OneLine_SendData(Nume/10);
		OneLine_SendData(Nume%10);
	}
	OneLine_SendData(Mode);
}

#else

/**************************串口控制************************************/

/************************************************************************
功能描述：组合播报函数
入口参数： 	*DAT：字符串指针,Len字符串长度
返 回 值： none
其他说明： 将需要播报的文件名放入数组中作为形参即可
**************************************************************************/
void  JQ_8x00_ZuHeBoFang(u8 *DATA,u8 Len)
{
	u16 CRC_data=0,i = 3;
	u8 Buffer[ZH_MAX] ={0xaa,0x1b};
	Buffer[2] = Len*2;			//计算长度
	CRC_data = CRC_data + 0xaa + 0x1b + Buffer[2];
	while(Len--)
	{
		Buffer[i] = *DATA/10+0x30;			//取出十位
		CRC_data = CRC_data + Buffer[i];
		i++;
		Buffer[i] = *DATA%10+0x30;			//取出个位
		CRC_data = CRC_data + Buffer[i];
		i++;
		DATA++;
	}
	Buffer[i] = CRC_data;
	i++;
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//忙检测
	#endif
	UART1_SendCode(Buffer,i);
}

/************************************************************************
功能描述：起始码-指令类型-数据长度-校验和
入口参数： 	MODE：模式
返 回 值： none
其他说明： 无数数据传入
**************************************************************************/
void  JQ_8x00_ModeSelect_0(UartDataZero MODE)
{
	u8 Buffer[4] ={0xaa};

    Buffer[1] = MODE;            //指令类型			
    Buffer[2] = 0x00;           //数据长度
    Buffer[3] = Buffer[0] +  Buffer[1] +  Buffer[2];            //校验和

	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//忙检测
	#endif
	UART1_SendCode(Buffer,4);
}

/************************************************************************
功能描述：起始码-指令类型-数据长度-数据-校验和
入口参数： 	*DAT：字符串指针,Len字符串长度
返 回 值： none
其他说明： 数据长度只能1或者0，当数据DATA为0时表示没有数据
**************************************************************************/
void  JQ_8x00_ModeSelect_1(UartDataOne MODE,u8 DATA)
{
	u8 Buffer[5] ={0xaa};
   
    Buffer[1] = MODE;       //指令类型    
    Buffer[2] = 1;			//数据长度
    Buffer[3] = DATA;       //数据
    Buffer[4] = Buffer[0] +  Buffer[1] +  Buffer[2] + Buffer[3];            //校验和
    
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//忙检测
	#endif
	UART1_SendCode(Buffer,5);
}


#endif
