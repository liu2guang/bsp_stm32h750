/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtquick.h" 
#include "board.h" 

//SPI_HandleTypeDef hspi4;

//void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hspi->Instance==SPI4)
//  {
//    __HAL_RCC_SPI4_CLK_ENABLE();
//  
//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    /**SPI4 GPIO Configuration    
//    PE12     ------> SPI4_SCK
//    PE13     ------> SPI4_MISO
//    PE14     ------> SPI4_MOSI 
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
//    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//  }
//}

//static void MX_SPI4_Init(void)
//{
//    hspi4.Instance = SPI4;
//    hspi4.Init.Mode = SPI_MODE_MASTER;
//    hspi4.Init.Direction = SPI_DIRECTION_2LINES;
//    hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
//    hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
//    hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
//    hspi4.Init.NSS = SPI_NSS_SOFT;
//    hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
//    hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
//    hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
//    hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//    hspi4.Init.CRCPolynomial = 7;
//    hspi4.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
//    hspi4.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
//    hspi4.Init.FifoThreshold = SPI_FIFO_THRESHOLD_08DATA;
//    hspi4.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//    hspi4.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
//    hspi4.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
//    hspi4.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
//    hspi4.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
//    hspi4.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
//    hspi4.Init.IOSwap = SPI_IO_SWAP_DISABLE;
//    HAL_SPI_Init(&hspi4); 
//}

int main(void)
{      
//    uint8_t pTxData[10] = {0x01, 0x02, 0x03, 0x04}; 
//    uint8_t pRxData[10] = {0}; 
//    
    rtquick_init();   

//    MX_SPI4_Init();
//    
//    __HAL_SPI_ENABLE(&hspi4); 
//    rt_kprintf("HAL_SPI_Transmit: %d\n", HAL_SPI_Transmit(&hspi4, pTxData, 4, 1000));
//    
//    __HAL_SPI_ENABLE(&hspi4); 
//    rt_kprintf("HAL_SPI_TransmitReceive: %d\n", HAL_SPI_TransmitReceive(&hspi4, pTxData, pRxData, 4, 1000));
    
    return 0; 
}
