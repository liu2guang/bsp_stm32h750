/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2019-03-03     liu2guang         the first version
 */
 
#include "drv_flash.h"
#include "board.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME "drv.flash"
#define DBG_LEVEL        DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#define _W25QXX_MODE_SPI (0) 
#define _W25QXX_MODE_QPI (1) 

#define _W25QXX_INSTRUCTION_EXIT_QPI_MODE  (0xFF) 
#define _W25QXX_INSTRUCTION_ENTER_QPI_MODE (0x38)
#define _W25QXX_INSTRUCTION_ENABLE_RESET   (0x66)
#define _W25QXX_INSTRUCTION_RESET_DEVICE   (0x99)
#define _W25QXX_SetReadParameters          (0xC0)
#define _W25QXX_FastRead                   (0x0B)
#define _W25QXX_ReadStatusReg1		       (0x05)
#define _W25QXX_ReadStatusReg2		       (0x35)
#define _W25QXX_ReadStatusReg3		       (0x15)
#define _W25QXX_WriteStatusReg1            (0x01) 
#define _W25QXX_WriteStatusReg2            (0x31)
#define _W25QXX_WriteStatusReg3            (0x11)
#define _W25QXX_WriteEnable                (0x06)
#define _W25QXX_VolatileSRWriteEnable      (0x50)
#define _W25QXX_WriteDisable               (0x04)

static QSPI_HandleTypeDef hqspi = {0}; 
static rt_uint8_t _w25qxx_mode = _W25QXX_MODE_SPI; 

rt_uint8_t _w25qxx_read_sr(rt_uint8_t srx)
{
    QSPI_CommandTypeDef cmd;
    rt_uint8_t data = 0;

    switch(_w25qxx_mode)
    {
    case _W25QXX_MODE_SPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE; 
        cmd.DataMode = QSPI_DATA_1_LINE;
        break; 

    case _W25QXX_MODE_QPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES; 
        cmd.DataMode = QSPI_DATA_4_LINES;
        break; 
    }

	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_24_BITS;
	cmd.Address = 0x00;	
	cmd.NbData = 1;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.AlternateBytesSize = 0;
	cmd.AlternateBytes = 0x00;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    switch(srx)
    {
    case 1:
        cmd.Instruction = _W25QXX_ReadStatusReg1;
        break;

    case 2:
        cmd.Instruction = _W25QXX_ReadStatusReg2;
        break;

    case 3:
        cmd.Instruction = _W25QXX_ReadStatusReg3;
        break;

    default:
        cmd.Instruction = _W25QXX_ReadStatusReg1;
        break; 
    }

	if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
	{
		HAL_QSPI_Receive(&hqspi, &data, 100);
	}

    return data; 
}

void _w25qxx_write_sr(rt_uint8_t srx, rt_uint8_t data)
{
    QSPI_CommandTypeDef cmd;

    switch(_w25qxx_mode)
    {
    case _W25QXX_MODE_SPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE; 
        cmd.DataMode = QSPI_DATA_1_LINE;
        break; 

    case _W25QXX_MODE_QPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES; 
        cmd.DataMode = QSPI_DATA_4_LINES;
        break; 
    }

	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_24_BITS;
	cmd.Address = 0x00;	
	cmd.NbData = 1;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.AlternateBytesSize = 0;
	cmd.AlternateBytes = 0x00;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    switch(srx)
    {
    case 1:
        cmd.Instruction = _W25QXX_WriteStatusReg1;
        break;

    case 2:
        cmd.Instruction = _W25QXX_WriteStatusReg2;
        break;

    case 3:
        cmd.Instruction = _W25QXX_WriteStatusReg3;
        break;

    default:
        cmd.Instruction = _W25QXX_WriteStatusReg1;
        break; 
    }

	if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
	{
		HAL_QSPI_Transmit(&hqspi, &data, 100);
	}
}

static void _w25qxx_write_enable(rt_bool_t en)
{
    QSPI_CommandTypeDef cmd;

    switch(_w25qxx_mode)
    {
    case _W25QXX_MODE_SPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE; 
        break; 

    case _W25QXX_MODE_QPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES; 
        break; 
    }

    switch(en)
    {
    case RT_TRUE: 
        cmd.Instruction = _W25QXX_WriteEnable; 
        break; 

    case RT_FALSE: 
        cmd.Instruction = _W25QXX_WriteDisable;
        break; 
    }

	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_24_BITS;
	cmd.Address = 0x00;	
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.NbData = 0;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.AlternateBytesSize = 0;
	cmd.AlternateBytes = 0x00;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, 100); 
}

static void _w25qxx_exit_QPI_mode(void)
{
    QSPI_CommandTypeDef cmd;

	cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_24_BITS;
	cmd.Address = 0x00;	
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.NbData = 0;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.AlternateBytesSize = 0;
	cmd.AlternateBytes = 0x00;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction = _W25QXX_INSTRUCTION_EXIT_QPI_MODE;
    HAL_QSPI_Command(&hqspi, &cmd, 100);

    _w25qxx_mode = _W25QXX_MODE_SPI; 
}

static void _w25qxx_enter_QPI_mode(void)
{
    QSPI_CommandTypeDef cmd; 
    rt_uint8_t data = 0;

    data = _w25qxx_read_sr(2); 
    if ((data & 0x02) == 0x00)
    {
        _w25qxx_write_enable(RT_TRUE); 
        data |= 0x02; 
        _w25qxx_write_sr(2, data); 
    }

	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_24_BITS;
	cmd.Address = 0x00;	
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.NbData = 0;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.AlternateBytesSize = 0;
	cmd.AlternateBytes = 0x00;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction = _W25QXX_INSTRUCTION_ENTER_QPI_MODE;
    HAL_QSPI_Command(&hqspi, &cmd, 100); 

    _w25qxx_mode = _W25QXX_MODE_QPI;

	cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	cmd.DataMode = QSPI_DATA_4_LINES;
	cmd.NbData = 1;
	data = 0x03 << 4; 

    _w25qxx_write_enable(RT_TRUE);

    cmd.Instruction = _W25QXX_SetReadParameters;
	if (HAL_QSPI_Command(&hqspi, &cmd, 100) == HAL_OK)
	{
		HAL_QSPI_Transmit(&hqspi, &data, 100);
	}
}

static rt_err_t _w25qxx_reset(void)
{
    QSPI_CommandTypeDef cmd;

    switch(_w25qxx_mode)
    {
    case _W25QXX_MODE_SPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE; 
        break; 

    case _W25QXX_MODE_QPI: 
        cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES; 
        break; 
    }

	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.AddressSize = QSPI_ADDRESS_24_BITS;
	cmd.Address = 0;
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.NbData = 0;
	cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	cmd.AlternateBytesSize = 0;
	cmd.AlternateBytes = 0x00;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* busy wait */ 
    while((_w25qxx_read_sr(1) & 0x01) == 0x01); 

    cmd.Instruction = _W25QXX_INSTRUCTION_ENABLE_RESET;
    if (HAL_QSPI_Command(&hqspi, &cmd, 100) != HAL_OK)
    {
        LOG_E("_W25QXX_INSTRUCTION_ENABLE_RESET send failed."); 
        return (-RT_ERROR);
    }

    cmd.Instruction = _W25QXX_INSTRUCTION_RESET_DEVICE;
    if (HAL_QSPI_Command(&hqspi, &cmd, 100) != HAL_OK)
    {
        LOG_E("_W25QXX_INSTRUCTION_RESET_DEVICE send failed."); 
        return (-RT_ERROR);
    }

    return RT_EOK; 
}

static void _w25qxx_mpaped_mode(void)
{
    QSPI_CommandTypeDef cmd;
    QSPI_MemoryMappedTypeDef mem;

    cmd.InstructionMode = QSPI_INSTRUCTION_4_LINES;
    cmd.AddressMode = QSPI_ADDRESS_4_LINES;
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.DataMode = QSPI_DATA_4_LINES;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DummyCycles = 8;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    mem.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    mem.TimeOutPeriod = 0;

    cmd.Instruction = _W25QXX_FastRead;
    HAL_QSPI_MemoryMapped(&hqspi, &cmd, &mem); 
}

static void _w25qxx_init(void)
{
    _w25qxx_exit_QPI_mode(); 
    _w25qxx_reset(); 
    _w25qxx_enter_QPI_mode(); 
}

int rt_hw_flash_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_QSPI_CLK_ENABLE();

    hqspi.Instance = QUADSPI;
    hqspi.Init.ClockPrescaler = 1; /* QSPI CLK = 200MHZ / (1+1) = 100MHz */ 
    hqspi.Init.FifoThreshold = 4;
    hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
    hqspi.Init.FlashSize = 22;
    hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
    hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
    hqspi.Init.FlashID = QSPI_FLASH_ID_1;
    hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;

    if (HAL_QSPI_Init(&hqspi) != HAL_OK)
    {
        LOG_E("flash init failed, qspi init failed."); 
        return (-RT_ERROR); 
    }

    _w25qxx_init();  
    _w25qxx_mpaped_mode(); 

    return RT_EOK; 
}
