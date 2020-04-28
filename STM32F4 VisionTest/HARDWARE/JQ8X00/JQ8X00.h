#ifndef __JQ8x00_H
#define __JQ8x00_H
#include "sys.h"

#define JQ8x00_Workmode		0		//模块工作方式，0表示串口控制，1表单线控制
#define JQ8x00_BusyCheck	0		//忙检测，0表示不做忙检测，1表做忙检测


/*
 * IO端口宏定义
 * 更换IO端口是修改这里的定义
 */
 //忙检测IO配置
#define JQ8x00_BUSY_RCC          RCC_AHB1Periph_GPIOA       //时钟
#define JQ8x00_BUSY_GPIO         GPIOA
#define JQ8x00_BUSY_GPIO_Pin     GPIO_Pin_5

//单线串口IO配置
#define JQ8x00_VPP_RCC           RCC_AHB1Periph_GPIOA        //时钟
#define JQ8x00_VPP_GPIO         GPIOA
#define JQ8x00_VPP_GPIO_Pin     GPIO_Pin_0                 //IO


//单线控制IO电平设置,忙检测
#define JQ8x00_BUSY  GPIO_ReadInputDataBit(JQ8x00_BUSY_GPIO,JQ8x00_BUSY_GPIO_Pin)		//读取电平

//单线控制IO电平设置，串口设置
#define JQ8x00_VPP_Clr()	 GPIO_ResetBits(JQ8x00_VPP_GPIO,JQ8x00_VPP_GPIO_Pin)
#define JQ8x00_VPP_Set()	 GPIO_SetBits(JQ8x00_VPP_GPIO,JQ8x00_VPP_GPIO_Pin)

#if JQ8x00_Workmode
/**********************单线控制**************************/
typedef enum {
    Play 		= 0x11,					/*播放*/
    Pause		= 0x12,					/*暂停*/
    Stop		= 0x13,					/*停止*/
    LastSong    = 0x14,					/*上一曲*/
    NextSong	= 0x15,					/*下一曲*/
    LastList	= 0x16,					/*上一目录*/
    NextList	= 0x17,					/*下一目录*/
    ChooseSD	= 0x18,					/*选择SD卡*/
    ChooseUdisk	= 0x19,					/*选择U盘*/
    ChooseFlash	= 0x1a,					/*选择Flash*/
    SysSleep	= 0x1b,					/*系统睡眠*/
} LineByteSelect;

typedef enum {
    Track 		= 0x0B,					/*曲目*/
    Volume		= 0x0C,					/*音量*/
    EQ			= 0x0D,					/*EQ*/
    LoopMode    = 0x0E,					/*循环模式*/
    Channel		= 0x0F,					/*通道*/
    CBTrack		= 0x10,					/*插拔曲目*/
} LineModeSelect;

void OneLine_ByteControl(LineByteSelect Mode);
void 
void OneLine_SendData(u8 DATA);

#else
/**********************串口控制**************************/
typedef enum {
    CheckPlayState                  = 0x01,					/*查询播报状态*/
    Play                            = 0x02,					/*播放*/
    pause                           = 0x03,					/*暂停*/
    Stop                            = 0x04,					/*停止*/
    LastSong                        = 0x05,					/*上一曲*/
    NextSong                        = 0x06,					/*下一曲*/
    NextList                        = 0x07,					/*下一目录*/
    CheckOnelineDisksign	        = 0x09,					/*查询当前在线盘符*/
    CheckCurrentDisksign	        = 0X0A,					/*查询当前播放盘符*/
    CheckTotalTrack                 = 0x0c,                 /*查询总曲目*/
    CurrentTrack                    = 0x0d,                  /*当前曲目*/
    LastFloder                      = 0x0e,                  /*上一个文件夹目录*/
    NextFloder                      = 0x0f,                  /*下一个文件夹目录*/          
    EndPlay	                        = 0x10, 				/*结束播放*/
    CheckFloderFirstTrack           = 0x11,                 /*查询文件目录首曲目*/
    CheckFloderAllTrack             = 0x12,                 /*查询文件目录总曲目*/
    AddVolume                       = 0x14,                 /*音量加*/
    DecVolume                       = 0x15,                 /*音量减*/
    SetEQ                           = 0x1a,                 /*设置EQ*/
    EndZHPlay                       = 0X1c,                 /*结束组合播报*/ 
    CheckSongShortName	            = 0x1E,					/*查询歌曲短文件名*/
    EndLoop                         = 0x21,                 /*结束复读*/
    GetTotalSongTime                = 0x24,                 /*获取当前曲目总时间*/
    OpenPlayTime                    = 0x25,                 /*播放时间开发送*/                
    ClosePlayTime                   = 0x26,                 /*关闭播放时间发送*/
}UartDataZero;     //无数据的指令,起始码-指令类型-数据长度-校验和

typedef enum {
    GoToDisksign                    = 0X0b,                 /*切换指定盘符*/
    SetVolume                       = 0x13,                 /*音量设置*/
    SetLoopMode                     = 0x18,                 /*设置循环模式*/
    SetChannel                      = 0x1d,                 /*设置通道*/  
}UartDataOne;       //包含一个数据的指令,起始码-指令类型-数据长度-数据1-校验和


void  JQ_8x00_ModeSelect_0(UartDataZero MODE);
void  JQ_8x00_ModeSelect_1(UartDataOne MODE,u8 DATA);
void JQ_8x00_ZuHeBoFang(u8 *DATA,u8 Len);

#endif


void JQ_8x00_Init(void);


#endif
