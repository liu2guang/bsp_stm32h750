/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "board.h" 

static UART_HandleTypeDef huart3;

static void rt_hw_uart_init(void)
{
    HAL_StatusTypeDef ret = HAL_OK;
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_USART3_CLK_ENABLE();
    
    GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    huart3.Instance = USART3;
    huart3.Init.BaudRate               = 115200;
    huart3.Init.WordLength             = UART_WORDLENGTH_8B;
    huart3.Init.StopBits               = UART_STOPBITS_1;
    huart3.Init.Parity                 = UART_PARITY_NONE;
    huart3.Init.Mode                   = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.Init.Prescaler              = UART_PRESCALER_DIV1;
    huart3.Init.FIFOMode               = UART_FIFOMODE_DISABLE;
    huart3.Init.TXFIFOThreshold        = UART_TXFIFO_THRESHOLD_1_8;
    huart3.Init.RXFIFOThreshold        = UART_RXFIFO_THRESHOLD_1_8;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    ret = HAL_UART_Init(&huart3); 
    if (ret != HAL_OK)
    {
        while(1);
    }
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart3 , (uint8_t *)&ch, 1 , 10);
    return ch;
}

void t1(void *p)
{
    while(1)
    {
        printf("Hello RTTGameBoard V1.0, Hello RTThread 1.\r\n"); 
        rt_thread_mdelay(1000); 
    }
}

void t2(void *p)
{
    while(1)
    {
        printf("Hello RTTGameBoard V1.0, Hello RTThread 2.\r\n"); 
        rt_thread_mdelay(1000); 
    }
}

int main(void)
{   
    rt_thread_t thread = RT_NULL; 
    
    rt_hw_uart_init(); 
    
    thread = rt_thread_create("t1", t1, RT_NULL, 1024,  9, 10); 
    rt_thread_startup(thread); 
    
    thread = rt_thread_create("t2", t2, RT_NULL, 1024, 10, 10); 
    rt_thread_startup(thread);   
    
    return 0; 
}
