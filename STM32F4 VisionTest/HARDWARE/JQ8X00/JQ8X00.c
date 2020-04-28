#include "JQ8X00.h"
#include "usart.h"	 
#include "delay.h"

#define ZH_MAX	50		//��ϲ����������

/************************************************************************
����������ģ��IO��ʼ������
��ڲ����� ��
�� �� ֵ�� none
����˵���� ������õ��߿���JQ8x00_Workmode��ֵ��Ϊ1
**************************************************************************/
void JQ_8x00_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(JQ8x00_BUSY_RCC | JQ8x00_VPP_RCC,ENABLE);	 //ʹ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin  = JQ8x00_BUSY_GPIO_Pin;		//JQ8X00æ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
 	GPIO_Init(JQ8x00_BUSY_GPIO, &GPIO_InitStructure);
	
	#if JQ8x00_Workmode
	GPIO_InitStructure.GPIO_Pin  = JQ8x00_VPP_GPIO_Pin;						//JQ8x00���߿���IO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(JQ8x00_VPP_GPIO, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.1
	GPIO_SetBits(JQ8x00_VPP_GPIO,JQ8x00_VPP_GPIO_Pin);						 	//�����
	#endif
}

/************************��������**************************************/
#if JQ8x00_Workmode						//���Ʒ�ʽѡ��

/******************************���߿���********************************/

/************************************************************************
����������	���߿��Ƶ��ֽڷ��ͺ���
��ڲ����� 	DATA����������
�� �� ֵ�� none
����˵���� ����Ҫ���͵�������Ϊ�βδ���
**************************************************************************/
void OneLine_SendData(u8 DATA)
{
	u8 i;
	
	JQ8x00_VPP_Set();		//��ʼ�ź�
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
����������	�����ֽڿ���
��ڲ����� 	Mode������
�� �� ֵ�� none
����˵���� ����Ҫ�Ĺ�����Ϊ�βδ���
**************************************************************************/
void OneLine_ByteControl(LineByteSelect Mode)
{
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//æ���
	#endif
	OneLine_SendData(Mode);
}

/************************************************************************
����������	���߿�����Ϸ��ͺ���
��ڲ����� 	Nume�����֣�Mode������
�� �� ֵ�� none
����˵���� ����Ҫ���͵����ݺ���Ҫ�Ĺ�����Ϊ�βδ���
**************************************************************************/
void OneLine_ZHControl(u8 Nume,LineModeSelect Mode)
{
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//æ���
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

/**************************���ڿ���************************************/

/************************************************************************
������������ϲ�������
��ڲ����� 	*DAT���ַ���ָ��,Len�ַ�������
�� �� ֵ�� none
����˵���� ����Ҫ�������ļ���������������Ϊ�βμ���
**************************************************************************/
void  JQ_8x00_ZuHeBoFang(u8 *DATA,u8 Len)
{
	u16 CRC_data=0,i = 3;
	u8 Buffer[ZH_MAX] ={0xaa,0x1b};
	Buffer[2] = Len*2;			//���㳤��
	CRC_data = CRC_data + 0xaa + 0x1b + Buffer[2];
	while(Len--)
	{
		Buffer[i] = *DATA/10+0x30;			//ȡ��ʮλ
		CRC_data = CRC_data + Buffer[i];
		i++;
		Buffer[i] = *DATA%10+0x30;			//ȡ����λ
		CRC_data = CRC_data + Buffer[i];
		i++;
		DATA++;
	}
	Buffer[i] = CRC_data;
	i++;
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//æ���
	#endif
	UART1_SendCode(Buffer,i);
}

/************************************************************************
������������ʼ��-ָ������-���ݳ���-У���
��ڲ����� 	MODE��ģʽ
�� �� ֵ�� none
����˵���� �������ݴ���
**************************************************************************/
void  JQ_8x00_ModeSelect_0(UartDataZero MODE)
{
	u8 Buffer[4] ={0xaa};

    Buffer[1] = MODE;            //ָ������			
    Buffer[2] = 0x00;           //���ݳ���
    Buffer[3] = Buffer[0] +  Buffer[1] +  Buffer[2];            //У���

	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//æ���
	#endif
	UART1_SendCode(Buffer,4);
}

/************************************************************************
������������ʼ��-ָ������-���ݳ���-����-У���
��ڲ����� 	*DAT���ַ���ָ��,Len�ַ�������
�� �� ֵ�� none
����˵���� ���ݳ���ֻ��1����0��������DATAΪ0ʱ��ʾû������
**************************************************************************/
void  JQ_8x00_ModeSelect_1(UartDataOne MODE,u8 DATA)
{
	u8 Buffer[5] ={0xaa};
   
    Buffer[1] = MODE;       //ָ������    
    Buffer[2] = 1;			//���ݳ���
    Buffer[3] = DATA;       //����
    Buffer[4] = Buffer[0] +  Buffer[1] +  Buffer[2] + Buffer[3];            //У���
    
	#if JQ8x00_BusyCheck
	while(JQ8x00_BUSY);				//æ���
	#endif
	UART1_SendCode(Buffer,5);
}


#endif
