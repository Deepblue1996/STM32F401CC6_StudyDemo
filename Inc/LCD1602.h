//
// Created by mayn on 2020/1/6 0006.
//

#ifndef HELLOSTM_LCD1602_H
#define HELLOSTM_LCD1602_H

#include "gpio.h"
#include "stm32f4xx_hal_gpio.h"

/***********************************���Ŷ���********************************************/
#define BUSY 0x80		//æ��־
#define RS rs_Pin	//����PB5ΪRS
#define RW rw_Pin	//PB6ΪRW
#define EN e_Pin	//PB7ΪENʹ��

#define P0 p0_Pin	//A
#define P1 p1_Pin	//A
#define P2 p2_Pin	//A
#define P3 p3_Pin	//B
#define P4 p4_Pin	//B
#define P5 p5_Pin	//B
#define P6 p6_Pin	//B
#define P7 p7_Pin	//B

#define u8 unsigned char
//PA0 ~ PA7��ӦD0 ~D7
/***********************************���Ŷ���********************************************/

/***********************************��������********************************************/
void GPIO_Configuration(void);
void LCD1602_Wait_Ready(void);
void LCD1602_Write_Cmd(u8 cmd);
void LCD1602_Write_Dat(u8 dat);
void LCD1602_ClearScreen(void);
void LCD1602_Set_Cursor(u8 x, u8 y);
void LCD1602_Show_Str(u8 x, u8 y, u8 *str);
void LCD1602_Init(void);
void LCD1602_Show_Str_Printf(u8 x, u8 y, u8 *str, ...);
void printfEx(const char *str, ...);
/***********************************��������********************************************/

#endif //HELLOSTM_LCD1602_H
