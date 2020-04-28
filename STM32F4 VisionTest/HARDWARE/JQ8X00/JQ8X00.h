#ifndef __JQ8x00_H
#define __JQ8x00_H
#include "sys.h"

#define JQ8x00_Workmode		0		//ģ�鹤����ʽ��0��ʾ���ڿ��ƣ�1���߿���
#define JQ8x00_BusyCheck	0		//æ��⣬0��ʾ����æ��⣬1����æ���


/*
 * IO�˿ں궨��
 * ����IO�˿����޸�����Ķ���
 */
 //æ���IO����
#define JQ8x00_BUSY_RCC          RCC_AHB1Periph_GPIOA       //ʱ��
#define JQ8x00_BUSY_GPIO         GPIOA
#define JQ8x00_BUSY_GPIO_Pin     GPIO_Pin_5

//���ߴ���IO����
#define JQ8x00_VPP_RCC           RCC_AHB1Periph_GPIOA        //ʱ��
#define JQ8x00_VPP_GPIO         GPIOA
#define JQ8x00_VPP_GPIO_Pin     GPIO_Pin_0                 //IO


//���߿���IO��ƽ����,æ���
#define JQ8x00_BUSY  GPIO_ReadInputDataBit(JQ8x00_BUSY_GPIO,JQ8x00_BUSY_GPIO_Pin)		//��ȡ��ƽ

//���߿���IO��ƽ���ã���������
#define JQ8x00_VPP_Clr()	 GPIO_ResetBits(JQ8x00_VPP_GPIO,JQ8x00_VPP_GPIO_Pin)
#define JQ8x00_VPP_Set()	 GPIO_SetBits(JQ8x00_VPP_GPIO,JQ8x00_VPP_GPIO_Pin)

#if JQ8x00_Workmode
/**********************���߿���**************************/
typedef enum {
    Play 		= 0x11,					/*����*/
    Pause		= 0x12,					/*��ͣ*/
    Stop		= 0x13,					/*ֹͣ*/
    LastSong    = 0x14,					/*��һ��*/
    NextSong	= 0x15,					/*��һ��*/
    LastList	= 0x16,					/*��һĿ¼*/
    NextList	= 0x17,					/*��һĿ¼*/
    ChooseSD	= 0x18,					/*ѡ��SD��*/
    ChooseUdisk	= 0x19,					/*ѡ��U��*/
    ChooseFlash	= 0x1a,					/*ѡ��Flash*/
    SysSleep	= 0x1b,					/*ϵͳ˯��*/
} LineByteSelect;

typedef enum {
    Track 		= 0x0B,					/*��Ŀ*/
    Volume		= 0x0C,					/*����*/
    EQ			= 0x0D,					/*EQ*/
    LoopMode    = 0x0E,					/*ѭ��ģʽ*/
    Channel		= 0x0F,					/*ͨ��*/
    CBTrack		= 0x10,					/*�����Ŀ*/
} LineModeSelect;

void OneLine_ByteControl(LineByteSelect Mode);
void 
void OneLine_SendData(u8 DATA);

#else
/**********************���ڿ���**************************/
typedef enum {
    CheckPlayState                  = 0x01,					/*��ѯ����״̬*/
    Play                            = 0x02,					/*����*/
    pause                           = 0x03,					/*��ͣ*/
    Stop                            = 0x04,					/*ֹͣ*/
    LastSong                        = 0x05,					/*��һ��*/
    NextSong                        = 0x06,					/*��һ��*/
    NextList                        = 0x07,					/*��һĿ¼*/
    CheckOnelineDisksign	        = 0x09,					/*��ѯ��ǰ�����̷�*/
    CheckCurrentDisksign	        = 0X0A,					/*��ѯ��ǰ�����̷�*/
    CheckTotalTrack                 = 0x0c,                 /*��ѯ����Ŀ*/
    CurrentTrack                    = 0x0d,                  /*��ǰ��Ŀ*/
    LastFloder                      = 0x0e,                  /*��һ���ļ���Ŀ¼*/
    NextFloder                      = 0x0f,                  /*��һ���ļ���Ŀ¼*/          
    EndPlay	                        = 0x10, 				/*��������*/
    CheckFloderFirstTrack           = 0x11,                 /*��ѯ�ļ�Ŀ¼����Ŀ*/
    CheckFloderAllTrack             = 0x12,                 /*��ѯ�ļ�Ŀ¼����Ŀ*/
    AddVolume                       = 0x14,                 /*������*/
    DecVolume                       = 0x15,                 /*������*/
    SetEQ                           = 0x1a,                 /*����EQ*/
    EndZHPlay                       = 0X1c,                 /*������ϲ���*/ 
    CheckSongShortName	            = 0x1E,					/*��ѯ�������ļ���*/
    EndLoop                         = 0x21,                 /*��������*/
    GetTotalSongTime                = 0x24,                 /*��ȡ��ǰ��Ŀ��ʱ��*/
    OpenPlayTime                    = 0x25,                 /*����ʱ�俪����*/                
    ClosePlayTime                   = 0x26,                 /*�رղ���ʱ�䷢��*/
}UartDataZero;     //�����ݵ�ָ��,��ʼ��-ָ������-���ݳ���-У���

typedef enum {
    GoToDisksign                    = 0X0b,                 /*�л�ָ���̷�*/
    SetVolume                       = 0x13,                 /*��������*/
    SetLoopMode                     = 0x18,                 /*����ѭ��ģʽ*/
    SetChannel                      = 0x1d,                 /*����ͨ��*/  
}UartDataOne;       //����һ�����ݵ�ָ��,��ʼ��-ָ������-���ݳ���-����1-У���


void  JQ_8x00_ModeSelect_0(UartDataZero MODE);
void  JQ_8x00_ModeSelect_1(UartDataOne MODE,u8 DATA);
void JQ_8x00_ZuHeBoFang(u8 *DATA,u8 Len);

#endif


void JQ_8x00_Init(void);


#endif
