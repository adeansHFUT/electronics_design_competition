//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3        
//				ģ��spiͨ��
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __BSP_OLED_H
#define __BSP_OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	 
#include "PORT_cfg.h"
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  

//****************OLEDģʽ0ʹ��*************************
//****************����ָ���*************************
#define OLED_CS_Clr()  GPIO_ResetBits(OLED_CS_PORT,OLED_CS_PIN)//CS
#define OLED_CS_Set()  GPIO_SetBits(OLED_CS_PORT,OLED_CS_PIN)

#define OLED_RST_Clr() GPIO_ResetBits(OLED_RST_PORT,OLED_RST_PIN)//RES
#define OLED_RST_Set() GPIO_SetBits(OLED_RST_PORT,OLED_RST_PIN)

#define OLED_DC_Clr() GPIO_ResetBits(OLED_DC_PORT,OLED_DC_PIN)//DC
#define OLED_DC_Set() GPIO_SetBits(OLED_DC_PORT,OLED_DC_PIN)
//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK_Clr() GPIO_ResetBits(OLED_SCLK_PORT,OLED_SCLK_PIN)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(OLED_SCLK_PORT,OLED_SCLK_PIN)

#define OLED_SDIN_Clr() GPIO_ResetBits(OLED_SDIN_PORT,OLED_SDIN_PIN)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(OLED_SDIN_PORT,OLED_SDIN_PIN)


//****************OLEDģʽ1ʹ��(û��)*************************
#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)

//PC0~7,��Ϊ������
#define DATAOUT(x) GPIO_Write(GPIOC,x);//���  



 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

//������ʾ�ڵ�
typedef struct show_node {         
	char srt[11];  // �ַ���
	u16 num;	// ����
}show_node;	

//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

u8 updatepage(show_node *page, u8 index, char *newstr, u16 newnum, u8 mode);
void showpage(show_node *page, unsigned char mode, unsigned char show_size);
u8 clearpage(show_node *page);
extern u8 autoRefresh;  // ȫ�ֻ�
#endif  

/*
��ʾ�������ӱ���
OLED_ShowCHinese(0,0,0); //
		OLED_ShowCHinese(18,0,1);
		OLED_ShowCHinese(36,0,2);
		OLED_ShowCHinese(54,0,3);//
		OLED_ShowCHinese(72,0,4);
		OLED_ShowCHinese(90,0,5);//
		OLED_ShowCHinese(108,0,6);//
��ʾͼƬ		
	OLED_DrawBMP(0,0,128,8,BMP1); 
*/


