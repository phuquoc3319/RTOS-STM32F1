#include "main.h"


/*
		when using rtos to devide tasks the main function dont need the while(1) anymore
		
		Remembering when configure for usart Rx pin always config it as input floating pin
		
		Function usart1_get_buffer still in modifying condition so can not use it at the moment
		Using usart interupt to receive data it will reduce losing data while transmitting
		
*/

// private variable
char send_buffer[100];
float a = 2.3225665;
float b = 145.21455;
float c = 2.325544;
float d = 2.14544;
float e = 89.32155;
float f = 2663.1552;
u16 data;
u8 high;
u8 low;
u16 combine;
volatile static u16 count;

// calling function
void led_init(void);
void usart1_send(u8 *p);
u8 usart1_get_char(void);
void usart1_send_char(u8 data);
void usart1_config(u32 baudrate);
void usart1_send_buffer(u8 *buffer,u16 length);
void usart1_get_buffer(u8 *buffer,u16 length);
unsigned char getLOWbyte(unsigned int input);
unsigned char getHIGHbyte(unsigned int input);
unsigned int combineBytes(unsigned char high, unsigned char low);
void timer_init(u16 psc, u16 arr);




// RTOS Task
void led_task(void *p);
void usart_task(void *p);
void sum_task(void *p);



int main(void)
{
	delay_init();
	led_init();
	sprintf(send_buffer,"Tracey %d %f\r\n",1986,3.88878995); // insert data to buffer
	usart1_config(115200);
	xTaskCreate(led_task,(const char*)"LED BLINK",256,NULL,1,NULL);
	xTaskCreate(usart_task,(const char *)"USART",256,NULL,1,NULL);
	xTaskCreate(sum_task,(const char *)"SUM",256,NULL,1,NULL); // create task
	timer_init(7200-1,10000-1);
	vTaskStartScheduler(); // Start parallel task
	
	return 0;
	
//	while(1)
//	{
//	}
}

/*
		function below to enable LED on PC13 pin
*/
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = (1<<13);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
}

/*
		function below is task for LED
*/
void led_task(void *p)
{
	while(1)
	{
//		GPIOC->ODR ^= (1<<13);
//		vTaskDelay(500/portTICK_PERIOD_MS);
		vTaskDelay(100);
	}
}

void usart_task(void *p)
{
	while(1)
	{
//		send_usart1("Hello World\r\n");
//		vTaskDelay(1);
//		usart_send_buffer("Hello World\r\n",strlen("Hello World\r\n"));
//		usart_send_buffer(send_buffer,1);
//		usart1_send_buffer(send_buffer,strlen(send_buffer));
//		vTaskDelay(500);
		
//		char receive[100];
//		char c = usart1_get_char();
//		
//		usart1_send_char(c);
		
//		vTaskDelay(100);
		
//		usart1_get_buffer(receive,100);
//		usart1_send_buffer(receive,strlen(receive));
		
		
		
	}
}

void sum_task(void *p)
{
	while(1)
	{
		a = a+0.0002152;
		b = b+0.00001;
		c = c+0.0002;
		d = d+0.001226;
		e = e+3.225412;
		f = f+52.1226522;
		vTaskDelay(100);
	}
	
}

void usart1_config(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = (1<<9); // Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = (1<<10); // Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1,ENABLE);
}


extern void USART1_IRQHandler(void)
{
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		char data = USART1->DR;
//		USART_SendData(USART1,data);
//	}
	
	data = USART1->DR;
//	data = USART_ReceiveData(USART1);
//	high = getHIGHbyte(data);
//	low = getLOWbyte(data);
//	combine = combineBytes(high,low);
	USART_SendData(USART1,data);
//	GPIOC->ODR ^= (1<<13);
}



void usart1_send_char(u8 data)
{
	USART_SendData(USART1,(uint8_t) data);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0);
}



u8 usart1_get_char(void)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==0);
	return USART_ReceiveData(USART1);
}

void usart1_send(u8 *p)
{
	while(*p)
	{
		usart1_send_char(*p++);
	}
}

void usart1_send_buffer(u8 *buffer,u16 length)
{
	while(length--)
	{
		USART_SendData(USART1,(u8)*buffer);
		buffer++;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==0);
	}
}

void usart1_get_buffer(u8 *buffer,u16 length)
{
	while(length--)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==0);
		*buffer = (uint8_t)USART_ReceiveData(USART1);
		if(*buffer == '\n')
		{
			break;
		}
		buffer++;
		
	}
}




unsigned char getLOWbyte(unsigned int input)
{
	return input & 255;
}
 
/* Get HIGH byte of int */
unsigned char getHIGHbyte(unsigned int input)
{
	return (input >> 8) & 255;
}
 
/* Combine HIGH and LOW bytes */
unsigned int combineBytes(unsigned char high, unsigned char low)
{
	return (int) (high << 8) + low;
}

void timer_init(u16 psc, u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM_Cmd(TIM4,ENABLE);
	
}

void TIM4_IRQHandler(void)
{
	count=0;
	/*
			in this case static variable will run correctly
			otherwise normal variable without static will not run correctly
	
	*/
	static u16 test=0;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==1)
	{
//		GPIOC->ODR ^= (1<<13);
		count++;
		test++;
		if(test>4)
		{
			GPIOC->ODR ^= (1<<13);
			test=0;
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}