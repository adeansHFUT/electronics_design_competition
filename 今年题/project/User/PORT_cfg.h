/*******************************************************************************
                              板子宏定义
*******************************************************************************/
// 在这里定义用的哪个开发板
//#define STM32f103ZET6_alien
#define STM32f103VET6_small   // 512k flash 64k Ram
//#define STM32f103C8T6_mostsmall

/*******************************************************************************
                              串口端口宏定义
*******************************************************************************/
/** 
  * ，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
//*****************串口1-USART1**********************/
#define  DEBUG_USARTx                   USART1   // 设备中定义(支持uart1~3,4要改dma)
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1 // 设备中定义
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd 
#define  DEBUG_USART_BAUDRATE           115200 // 设备中定义
// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA) // 设备中定义
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   // 设备中定义
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9 // 设备中定义
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA	// 设备中定义
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10	// 设备中定义
#define  DEBUG_USART_IRQ                USART1_IRQn	// 设备中定义
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler
// 串口对应的DMA请求通道
#define  DEBUG_USART_RX_DMA_CHANNEL     DMA1_Channel5 // 设备中定义
// 外设寄存器地址
#define  DEBUG_USART_DR_ADDRESS        (&DEBUG_USARTx->DR) // 设备中定义
// 一次接收的数据量
#define  DEBUG_USART_RBUFF_SIZE         1000  // 设备中定义

//*****************串口12-USART2**********************/
#define  Camera_USARTx                   USART2
#define  Camera_USART_CLK                RCC_APB1Periph_USART2
#define  Camera_USART_APBxClkCmd         RCC_APB1PeriphClockCmd // 设备中不定义
#define  Camera_USART_BAUDRATE           115200
// USART GPIO 引脚宏定义
#define  Camera_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  Camera_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd // 设备中不定义
#define  Camera_USART_TX_GPIO_PORT       GPIOA   
#define  Camera_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  Camera_USART_RX_GPIO_PORT       GPIOA
#define  Camera_USART_RX_GPIO_PIN        GPIO_Pin_3
#define  Camera_USART_IRQ                USART2_IRQn
#define  Camera_USART_IRQHandler         USART2_IRQHandler // 设备中不定义
// 串口对应的DMA请求通道
#define  Camera_USART_RX_DMA_CHANNEL     DMA1_Channel6
// 外设寄存器地址
#define  Camera_USART_DR_ADDRESS        (&Camera_USARTx->DR)
// 所有串口一次最多接收的数据量
#define  Camera_USART_RBUFF_SIZE         1000  
/*******************************************************************************
                              AT24C02端口定义
*******************************************************************************/

#if defined(STM32f103VET6_small)
/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_4)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOE
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_5)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO操作函数	 
#define AT24cxx_IIC_SCL    PEout(4) //SCL
#define AT24cxx_IIC_SDA    PEout(5) //SDA	 
#define AT24cxx_READ_SDA   PEin(5)  //输入SDA

#endif
#if defined(STM32f103ZET6_alien)
/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO操作函数	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //输入SDA
#endif
/*******************************************************************************
                              按键端口宏定义
*******************************************************************************/
#if defined(STM32f103VET6_small) 
#define KEY1_Pin   			GPIO_Pin_4    //定义K_1管脚
#define KEY2_Pin    		GPIO_Pin_5    //定义K_2管脚
#define KEY3_Pin   	 		GPIO_Pin_6   //定义K_3管脚
#define KEY4_Pin   		    GPIO_Pin_7  //定义K_4管脚
#define KEY5_Pin   			GPIO_Pin_8    //定义K_5管脚
#define KEY6_Pin    		GPIO_Pin_9    //定义K_6管脚
#define KEY7_Pin   	 		GPIO_Pin_0   //定义K_7管脚
#define KEY8_Pin   		    GPIO_Pin_1  //定义K_8管脚
#define KEY1_Port   		GPIOB //定义端口
#define KEY1_port_RCC 		RCC_APB2Periph_GPIOB
#define KEY2_Port   		GPIOB //定义端口
#define KEY2_port_RCC 		RCC_APB2Periph_GPIOB
#define KEY3_Port  			GPIOB //定义端口
#define KEY3_port_RCC 		RCC_APB2Periph_GPIOB
#define KEY4_Port   		GPIOB //定义端口
#define KEY4_port_RCC 		RCC_APB2Periph_GPIOB
#define KEY5_Port   		GPIOB //定义端口
#define KEY5_port_RCC 		RCC_APB2Periph_GPIOB
#define KEY6_Port   		GPIOB //定义端口
#define KEY6_port_RCC 		RCC_APB2Periph_GPIOB
#define KEY7_Port   		GPIOE //定义端口
#define KEY7_port_RCC 		RCC_APB2Periph_GPIOE
#define KEY8_Port   		GPIOE //定义端口
#define KEY8_port_RCC 		RCC_APB2Periph_GPIOE
//使用位操作定义
#define K_1 PBin(4)
#define K_2 PBin(5)
#define K_3 PBin(6)
#define K_4 PBin(7)
#define K_5 PBin(8)
#define K_6 PBin(9)
#define K_7 PEin(0)
#define K_8 PEin(1)
#endif
#if defined(STM32f103ZET6_alien) 
#define KEY1_Pin   		    GPIO_Pin_1    //定义K_1管脚
#define KEY2_Pin    	    GPIO_Pin_2    //定义K_2管脚
#define KEY3_Pin 		    GPIO_Pin_3   //定义K_3管脚
#define KEY4_Pin    	    GPIO_Pin_4  //定义K_4管脚
#define KEY1_Port   	    GPIOD //定义端口
#define KEY1_port_RCC 		RCC_APB2Periph_GPIOD
#define KEY2_Port   		GPIOD //定义端口
#define KEY2_port_RCC 		RCC_APB2Periph_GPIOD
#define KEY3_Port   		GPIOD //定义端口
#define KEY3_port_RCC 		RCC_APB2Periph_GPIOD
#define KEY4_Port  			GPIOD //定义端口
#define KEY4_port_RCC 		RCC_APB2Periph_GPIOD
//使用位操作定义
#define K_1 PDin(1)
#define K_2 PDin(2)
#define K_3 PDin(3)
#define K_4 PDin(4)
#endif
/*******************************************************************************
                              OLED端口宏定义
*******************************************************************************/
//-----------------OLED端口定义----------------  
#if defined(STM32f103VET6_small)
#define OLED_CS_PORT 	 		 GPIOB
#define OLED_RCC_CS_PORT  	     RCC_APB2Periph_GPIOB
#define OLED_CS_PIN  			 GPIO_Pin_10

#define OLED_DC_PORT  			 GPIOB
#define OLED_RCC_DC_PORT  		 RCC_APB2Periph_GPIOB
#define OLED_DC_PIN   			 GPIO_Pin_11

#define OLED_RST_PORT  			 GPIOB
#define OLED_RCC_RST_PORT        RCC_APB2Periph_GPIOB
#define OLED_RST_PIN  			 GPIO_Pin_12

#define OLED_SDIN_PORT     		 GPIOB                          //SDIN(D1
#define OLED_RCC_SDIN_PORT  	 RCC_APB2Periph_GPIOB
#define OLED_SDIN_PIN 			 GPIO_Pin_13

#define OLED_SCLK_PORT 			 GPIOB
#define OLED_RCC_SCLK_PORT  	 RCC_APB2Periph_GPIOB            //SCLK(D0)
#define OLED_SCLK_PIN 			 GPIO_Pin_14

#endif
#if defined(STM32f103ZET6_alien)
#define OLED_CS_PORT 			 GPIOC
#define OLED_RCC_CS_PORT  		 RCC_APB2Periph_GPIOC
#define OLED_CS_PIN   			 GPIO_Pin_13

#define OLED_RST_PORT  			 GPIOE
#define OLED_RCC_RST_PORT 		 RCC_APB2Periph_GPIOE
#define OLED_RST_PIN  			 GPIO_Pin_3

#define OLED_DC_PORT  			 GPIOE
#define OLED_RCC_DC_PORT 		 RCC_APB2Periph_GPIOE
#define OLED_DC_PIN  			 GPIO_Pin_5            

#define OLED_SCLK_PORT 			 GPIOB                              //SCLK(D0)
#define OLED_RCC_SCLK_PORT  	 RCC_APB2Periph_GPIOB
#define OLED_SCLK_PIN  			 GPIO_Pin_9

#define OLED_SDIN_PORT 			 GPIOE                             //SDIN(D1)
#define OLED_RCC_SDIN_PORT  	 RCC_APB2Periph_GPIOE
#define OLED_SDIN_PIN  			 GPIO_Pin_1
#endif
/*******************************************************************************
                              舵机端口宏定义
*******************************************************************************/
#define Steer1_S3010_mid  765        // 单位:万分之一
#define Steer1_S3010_max  1218   
#define Steer1_S3010_min  270
#define Steer2_S3010_mid  744   
#define Steer2_S3010_max  1218   // 最左边
#define Steer2_S3010_min  270
#if defined(STM32f103VET6_small)
#define steer1_port 			 GPIOA             // 初始化函数暂不支持重映射
#define steer1_port_RCC 		 RCC_APB2Periph_GPIOA
#define steer1_pin 				 GPIO_Pin_6
#define steer1_timx 			 TIM3  
#define steer1_timx_RCC		     RCC_APB1Periph_TIM3
#define steer1_TIM_channel 		 TIM_Channel_1

#define steer2_port 			 GPIOA             // 初始化函数暂不支持重映射
#define steer2_port_RCC 		 RCC_APB2Periph_GPIOA
#define steer2_pin 				 GPIO_Pin_7
#define steer2_timx 			 TIM3  
#define steer2_timx_RCC		     RCC_APB1Periph_TIM3
#define steer2_TIM_channel 		 TIM_Channel_2
#endif
#if defined(STM32f103ZET6_alien)
#define steer1_port 			 GPIOA             // 初始化函数暂不支持重映射
#define steer1_port_RCC 		 RCC_APB2Periph_GPIOA
#define steer1_pin 				 GPIO_Pin_6
#define steer1_timx 			 TIM3 
#define steer1_tim_RCC 			 RCC_APB1Periph_TIM3
#define steer1_TIM_channel 		 TIM_Channel_1

#define steer2_port 			 GPIOA             // 初始化函数暂不支持重映射
#define steer2_port_RCC 		 RCC_APB2Periph_GPIOA
#define steer2_pin 				 GPIO_Pin_7
#define steer2_timx 			 TIM3  
#define steer2_tim_RCC 			 RCC_APB1Periph_TIM3
#define steer2_TIM_channel 		 TIM_Channel_2
#endif

