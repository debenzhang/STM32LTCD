/*
 * touch_iic.h
 *
 *  Created on: Dec 1, 2020
 *      Author: laoxi
 */

#ifndef INC_TOUCH_IIC_H_
#define INC_TOUCH_IIC_H_

#include "stm32f4xx.h"

/*---------------------------------- IIC 引脚配置宏 ---------------------------------*/


#define Touch_IIC_SCL_CLK_ENABLE     __HAL_RCC_GPIOH_CLK_ENABLE()		// SCL 引脚时钟
#define Touch_IIC_SCL_PORT   			 GPIOH                 				// SCL 引脚端口
#define Touch_IIC_SCL_PIN     		 GPIO_PIN_4  							// SCL 引脚

#define Touch_IIC_SDA_CLK_ENABLE     __HAL_RCC_GPIOH_CLK_ENABLE() 	// SDA 引脚时钟
#define Touch_IIC_SDA_PORT   			 GPIOH                   			// SDA 引脚端口
#define Touch_IIC_SDA_PIN    			 GPIO_PIN_5              			// SDA 引脚

#define Touch_INT_CLK_ENABLE    		 __HAL_RCC_GPIOI_CLK_ENABLE()		// INT 引脚时钟
#define Touch_INT_PORT   				 GPIOI                   			// INT 引脚端口
#define Touch_INT_PIN    				 GPIO_PIN_11             			// INT 引脚

#define Touch_RST_CLK_ENABLE   		 __HAL_RCC_GPIOC_CLK_ENABLE()		// RST 引脚时钟
#define Touch_RST_PORT   				 GPIOC                   			// RST 引脚端口
#define Touch_RST_PIN    				 GPIO_PIN_13           				// RST 引脚

/*----------------------------------- IIC相关定义 ----------------------------------*/

#define ACK_OK  	1  			// 响应正常
#define ACK_ERR 	0				// 响应错误

// IIC通信延时，Touch_IIC_Delay()函数使用，
//	通信速度在100KHz左右，由于使用较长的FPC排线连接屏幕，不建议使用过高的速度
#define IIC_DelayVaule  20

/*------------------------------------ IO口操作 ------------------------------------*/


#define Touch_IIC_SCL(a)	if (a)	\
										HAL_GPIO_WritePin(Touch_IIC_SCL_PORT, Touch_IIC_SCL_PIN, GPIO_PIN_SET); \
									else		\
										HAL_GPIO_WritePin(Touch_IIC_SCL_PORT, Touch_IIC_SCL_PIN, GPIO_PIN_RESET)

#define Touch_IIC_SDA(a)	if (a)	\
										HAL_GPIO_WritePin(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, GPIO_PIN_SET); \
									else		\
										HAL_GPIO_WritePin(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, GPIO_PIN_RESET)

/*------------------------------------ 函数声明 -----------------------------------*/

void 	Touch_IIC_GPIO_Config (void);			// IIC引脚初始化
void 	Touch_IIC_Delay(uint32_t a);					// IIC延时函数
void	Touch_INT_Out(void);						// INT数据脚配置为输出模式
void	Touch_INT_In(void);						// INT数据脚配置为输入模式
void 	Touch_IIC_Start(void);					// 启动IIC通信
void 	Touch_IIC_Stop(void);					// IIC停止信号
void 	Touch_IIC_ACK(void);						//	发送响应信号
void 	Touch_IIC_NoACK(void);					// 发送非应答信号
uint8_t 	Touch_IIC_WaitACK(void);				//	等待应答信号
uint8_t	 	Touch_IIC_WriteByte(uint8_t IIC_Data); 	// 写字节函数
uint8_t 	Touch_IIC_ReadByte(uint8_t ACK_Mode);		// 读字节函数



#endif /* INC_TOUCH_IIC_H_ */
