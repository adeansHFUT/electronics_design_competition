#ifndef __BSP_USART_H
#define	__BSP_USART_H


#include "stm32f10x.h"
#include <stdio.h>
#include "PORT_cfg.h"


// ��iic��������������˼�빹��uart,֮����˵
typedef struct  
{
	USART_TypeDef* uart_module;  //uartģ��
	GPIO_TypeDef* gpio_port;  //gpio�˿�
	uint16_t gpio_pin;  // gpio����
	uint16_t irq;  // �ĸ�ģ���irq
	uint32_t RCC_uart;  //uart�ĸ�RCCʱ��
	uint32_t RCC_gpio;  // gpio�ĸ�RCCʱ��
	uint32_t baudrate;  // �����ʶ���
	DMA_Channel_TypeDef* dma_channel;  // dmaͨ���ĸ�
	uint32_t dma_address; // �����ַ
	uint32_t buffer_size; // һ�η��͵�������

}My_uart;	
// ����2-USART2
//#define  DEBUG_USARTx                   USART2
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  DEBUG_USART_IRQ                USART2_IRQn
//#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

// ����3-USART3
//#define  DEBUG_USARTx                   USART3
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOB   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  DEBUG_USART_IRQ                USART3_IRQn
//#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

// ����4-UART4
//#define  DEBUG_USARTx                   UART4
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  DEBUG_USART_IRQ                UART4_IRQn
//#define  DEBUG_USART_IRQHandler         UART4_IRQHandler


// ����5-UART5
//#define  DEBUG_USARTx                   UART5
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_12
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

//#define  DEBUG_USART_IRQ                UART5_IRQn
//#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


void USART_Config(void);
void USARTx_DMA_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Uart_DMA_Rx_Data(void);
void DEBUG_USART_IRQHandler(void);
extern char Usart_Rx_Buf[USART_RBUFF_SIZE];
#endif /* __USART_H */
