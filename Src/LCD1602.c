//
// Created by mayn on 2020/1/6 0006.
//

#include "LCD1602.h"
#include "stdio.h"
#include "stdarg.h"

//1602指令/数据 引脚
void LCD_RS_Set() {
    HAL_GPIO_WritePin(GPIOB, rs_Pin, GPIO_PIN_SET);
}

void LCD_RS_Clr(){
    HAL_GPIO_WritePin(GPIOB, rs_Pin, GPIO_PIN_RESET);
}

//1602读写引脚
void LCD_RW_Set() {
    HAL_GPIO_WritePin(GPIOB, rw_Pin, GPIO_PIN_SET);
}

void LCD_RW_Clr(){
    HAL_GPIO_WritePin(GPIOB, rw_Pin, GPIO_PIN_RESET);
}

//1602使能引脚
void LCD_EN_Set() {
    HAL_GPIO_WritePin(GPIOB, e_Pin, GPIO_PIN_SET);
}

void LCD_EN_Clr(){
    HAL_GPIO_WritePin(GPIOB, e_Pin, GPIO_PIN_RESET);
}

int GetBit(char c,int i)//取c的第i位
{
    return ( c >> i ) & 1;
}

void DATAOUT(unsigned char CMD) {
    HAL_GPIO_WritePin(GPIOA, P0, GetBit(CMD, 0));
    HAL_GPIO_WritePin(GPIOA, P1, GetBit(CMD, 1));
    HAL_GPIO_WritePin(GPIOA, P2, GetBit(CMD, 2));
    HAL_GPIO_WritePin(GPIOB, P3, GetBit(CMD, 3));
    HAL_GPIO_WritePin(GPIOB, P4, GetBit(CMD, 4));
    HAL_GPIO_WritePin(GPIOB, P5, GetBit(CMD, 5));
    HAL_GPIO_WritePin(GPIOB, P6, GetBit(CMD, 6));
    HAL_GPIO_WritePin(GPIOB, P7, GetBit(CMD, 7));
}

/* 检测液晶忙不忙 */
void LCD1602_Wait_Ready(void)
{
    int8_t sta;

    DATAOUT(0xff);    //PA端口全部置1
    LCD_RS_Clr();     //RS 0
    LCD_RW_Set();     //RW 1
    do
    {
        LCD_EN_Set();   //EN 1
        HAL_Delay(5);	//延时5MS
        sta = HAL_GPIO_ReadPin(GPIOB, P7);//读取状态字
        LCD_EN_Clr();  //EN  0
    }while(sta & BUSY);//bit7等于1表示忙,重复检测到其为0停止
}

/*向1602写入一字节命令,cmd-待写入命令 */
//写入指令PA4-7 RS-0  RW-0
void LCD1602_Write_Cmd(u8 cmd)
{
    LCD1602_Wait_Ready();  //判断忙不忙
    LCD_RS_Clr();
    LCD_RW_Clr();
    DATAOUT(cmd);  //先传高四位
    LCD_EN_Set();
    LCD_EN_Clr();

    DATAOUT(cmd<<4); //低四位左移到高四位
    LCD_EN_Set();
    LCD_EN_Clr();
}

/* 向1602写入一字节数据*/
void LCD1602_Write_Dat(u8 dat)
{
    LCD1602_Wait_Ready(); //判断忙不忙
    LCD_RS_Set();   //1
    LCD_RW_Clr();   //0

    DATAOUT(dat);   //先传高四位
    LCD_EN_Set();
    LCD_EN_Clr();

    DATAOUT(dat<<4); //低四位左移到高四位
    LCD_EN_Set();
    LCD_EN_Clr();

}

/*清屏*/
void LCD1602_ClearScreen(void)
{
    LCD1602_Write_Cmd(0x01);

}

/* 设置RAM其实地址,即光标位置,（X,Y）对应屏幕上的字符坐标 */
void LCD1602_Set_Cursor(u8 x, u8 y)
{
    u8 addr;

    if (y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    LCD1602_Write_Cmd(addr | 0x80);
}

/* 在液晶上显示字符串,（X,Y）-对应屏幕上的其实坐标，str-字符串指针 */
void LCD1602_Show_Str(u8 x, u8 y, u8 *str)
{
    LCD1602_Set_Cursor(x, y);
    while(*str != '\0')
    {
        LCD1602_Write_Dat(*str++);
    }
}

/* 初始化液晶 */
void LCD1602_Init(void)
{
    LCD1602_Write_Cmd(0x28);	//16*2显示，5*7点阵，4位数据口
    LCD1602_Write_Cmd(0x0C);	//开显示，光标关闭
    LCD1602_Write_Cmd(0x06);	//文字不动，地址自动+1
    LCD1602_Write_Cmd(0x01);	//清屏
}

void LCD1602_Show_Str_Printf(u8 x, u8 y, u8 *str, ...) {
    u8 pStr[sizeof(str)*10];
    va_list args;
    int n;

    va_start(args, str);
    n = vsprintf(pStr, str, args);
    va_end(args);

    LCD1602_Show_Str(x, y, pStr);
}