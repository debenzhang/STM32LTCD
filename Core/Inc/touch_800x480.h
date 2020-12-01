/*
 * touch_800x480.h
 *
 *  Created on: Dec 1, 2020
 *      Author: laoxi
 */

#ifndef INC_TOUCH_800X480_H_
#define INC_TOUCH_800X480_H_

#include "stm32f4xx.h"
#include "touch_iic.h"
#include "usart.h"

/*------------------------------------ 相关定义 -----------------------------------*/

#define TOUCH_MAX   5	//最大触摸点数

typedef struct
{
	uint8_t  flag;			//	触摸标志位，为1时表示有触摸操作
	uint8_t  num;				//	触摸点数
	uint16_t x[TOUCH_MAX];	//	x坐标
	uint16_t y[TOUCH_MAX];	//	y坐标
}TouchStructure;

extern TouchStructure touchInfo;	// 触摸数据结构体声明

/*------------------------------------ 寄存定义 -----------------------------------*/

#define GT9XX_IIC_RADDR 0xBB			// IIC初始化地址
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_READ_ADDR 0x814E		// 触摸信息寄存器，两款芯片的地址一样
#define GT9XX_ID_ADDR 	0x8140		// 触摸面板ID寄存器，两款芯片的地址一样


#define GT9XX_CFG_ADDR 	0x8047		// 固件配置信息寄存器和配置起始地址
#define GT9XX_Chksum_ADDR  0X80FF	// GT911 校验和寄存器

#define GT5688_CFG_ADDR 	0x8050		// GT5688 固件信息寄存器和配置寄存器起始地址
#define GT5688_Chksum_ADDR 0x813C		// GT5688 校验和寄存器

/*------------------------------------ 函数声明 -----------------------------------*/

uint8_t		Touch_Init(void);		// 触摸屏初始化
void 		Touch_Scan(void);		// 触摸扫描
void		GT9XX_Reset(void);	// 执行复位操作
void 		GT9XX_SendCfg(void);	// 发送GT911配置参数
void 		GT9XX_ReadCfg(void);	// 读取GT911配置参数



#endif /* INC_TOUCH_800X480_H_ */
