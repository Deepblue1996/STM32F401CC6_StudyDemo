//
// Created by mayn on 2020/1/6 0006.
//

#include "LCD1602.h"
#include "stdio.h"
#include "stdarg.h"

//1602ָ��/���� ����
void LCD_RS_Set() {
    HAL_GPIO_WritePin(GPIOB, rs_Pin, GPIO_PIN_SET);
}

void LCD_RS_Clr(){
    HAL_GPIO_WritePin(GPIOB, rs_Pin, GPIO_PIN_RESET);
}

//1602��д����
void LCD_RW_Set() {
    HAL_GPIO_WritePin(GPIOB, rw_Pin, GPIO_PIN_SET);
}

void LCD_RW_Clr(){
    HAL_GPIO_WritePin(GPIOB, rw_Pin, GPIO_PIN_RESET);
}

//1602ʹ������
void LCD_EN_Set() {
    HAL_GPIO_WritePin(GPIOB, e_Pin, GPIO_PIN_SET);
}

void LCD_EN_Clr(){
    HAL_GPIO_WritePin(GPIOB, e_Pin, GPIO_PIN_RESET);
}

int GetBit(char c,int i)//ȡc�ĵ�iλ
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

/* ���Һ��æ��æ */
void LCD1602_Wait_Ready(void)
{
    int8_t sta;

    DATAOUT(0xff);    //PA�˿�ȫ����1
    LCD_RS_Clr();     //RS 0
    LCD_RW_Set();     //RW 1
    do
    {
        LCD_EN_Set();   //EN 1
        HAL_Delay(5);	//��ʱ5MS
        sta = HAL_GPIO_ReadPin(GPIOB, P7);//��ȡ״̬��
        LCD_EN_Clr();  //EN  0
    }while(sta & BUSY);//bit7����1��ʾæ,�ظ���⵽��Ϊ0ֹͣ
}

/*��1602д��һ�ֽ�����,cmd-��д������ */
//д��ָ��PA4-7 RS-0  RW-0
void LCD1602_Write_Cmd(u8 cmd)
{
    LCD1602_Wait_Ready();  //�ж�æ��æ
    LCD_RS_Clr();
    LCD_RW_Clr();
    DATAOUT(cmd);  //�ȴ�����λ
    LCD_EN_Set();
    LCD_EN_Clr();

    DATAOUT(cmd<<4); //����λ���Ƶ�����λ
    LCD_EN_Set();
    LCD_EN_Clr();
}

/* ��1602д��һ�ֽ�����*/
void LCD1602_Write_Dat(u8 dat)
{
    LCD1602_Wait_Ready(); //�ж�æ��æ
    LCD_RS_Set();   //1
    LCD_RW_Clr();   //0

    DATAOUT(dat);   //�ȴ�����λ
    LCD_EN_Set();
    LCD_EN_Clr();

    DATAOUT(dat<<4); //����λ���Ƶ�����λ
    LCD_EN_Set();
    LCD_EN_Clr();

}

/*����*/
void LCD1602_ClearScreen(void)
{
    LCD1602_Write_Cmd(0x01);

}

/* ����RAM��ʵ��ַ,�����λ��,��X,Y����Ӧ��Ļ�ϵ��ַ����� */
void LCD1602_Set_Cursor(u8 x, u8 y)
{
    u8 addr;

    if (y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    LCD1602_Write_Cmd(addr | 0x80);
}

/* ��Һ������ʾ�ַ���,��X,Y��-��Ӧ��Ļ�ϵ���ʵ���꣬str-�ַ���ָ�� */
void LCD1602_Show_Str(u8 x, u8 y, u8 *str)
{
    LCD1602_Set_Cursor(x, y);
    while(*str != '\0')
    {
        LCD1602_Write_Dat(*str++);
    }
}

/* ��ʼ��Һ�� */
void LCD1602_Init(void)
{
    LCD1602_Write_Cmd(0x28);	//16*2��ʾ��5*7����4λ���ݿ�
    LCD1602_Write_Cmd(0x0C);	//����ʾ�����ر�
    LCD1602_Write_Cmd(0x06);	//���ֲ�������ַ�Զ�+1
    LCD1602_Write_Cmd(0x01);	//����
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