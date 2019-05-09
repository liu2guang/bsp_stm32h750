/*
 * Copyright (c) 2019-2019, liu2guang
 *
 * "Anti 996" License Version 1.0 (Draft) 
 */ 

#include "drv_psram.h"
#include "board.h"

#define DBG_ENABLE
#define DBG_SECTION_NAME "drv.psram"
#define DBG_LEVEL        DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

/* PSRAM IPS6404L CMD */ 
#define PSRAM_IPS6404L_CMD_READ            (0x03)
#define PSRAM_IPS6404L_CMD_FAST_READ       (0x0B)
#define PSRAM_IPS6404L_CMD_FAST_READ_QUAD  (0xEB)
#define PSRAM_IPS6404L_CMD_WRITE           (0x02)
#define PSRAM_IPS6404L_CMD_WRITE_QUAD      (0x38)
#define PSRAM_IPS6404L_CMD_ENTER_QUAD_MODE (0x35)
#define PSRAM_IPS6404L_CMD_EXIT_QUAD_MODE  (0xF5)
#define PSRAM_IPS6404L_CMD_RESER_ENABLE    (0x66)
#define PSRAM_IPS6404L_CMD_RESER           (0x99)
#define PSRAM_IPS6404L_CMD_READ_ID         (0x9F)

static QSPI_HandleTypeDef hqspi = {0};

static rt_err_t stm32_psram_reset(void)
{
    QSPI_CommandTypeDef cmd;

    cmd.Instruction       = PSRAM_IPS6404L_CMD_RESER_ENABLE;
    cmd.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode       = QSPI_ADDRESS_NONE;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode          = QSPI_DATA_NONE;
    cmd.DummyCycles       = 0;
    cmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("PSRAM_IPS6404L_CMD_RESER_ENABLE send failed."); 
        return (-RT_ERROR);
    }

    cmd.Instruction = PSRAM_IPS6404L_CMD_RESER;
    if(HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("PSRAM_IPS6404L_CMD_RESER send failed."); 
        return (-RT_ERROR);
    }

    return RT_EOK; 
}

static rt_err_t stm32_psram_check(void)
{
    QSPI_CommandTypeDef cmd;

    rt_uint8_t id[8] = {0}; 

    /* Reading ID Sequence */ 
    cmd.Instruction        = PSRAM_IPS6404L_CMD_READ_ID;
    cmd.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode        = QSPI_ADDRESS_NONE; 
    cmd.AddressSize        = QSPI_ADDRESS_24_BITS;
    cmd.Address            = 0x00;
    cmd.DataMode           = QSPI_DATA_1_LINE; 
    cmd.NbData             = 8; 
    cmd.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0; 
    cmd.AlternateBytes     = 0x00; 
    cmd.DummyCycles        = 0; 
    cmd.DdrMode            = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("PSRAM_IPS6404L_CMD_READ_ID send failed."); 
        return (-RT_ERROR);
    }

    if(HAL_QSPI_Receive(&hqspi, id, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("PSRAM_IPS6404L_CMD_READ_ID read id failed."); 
        return (-RT_ERROR);
    }

    LOG_D("PSRAM ID: 0x%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X.", 
        id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]); 

    if(id[0] != 0x00 || id[1] != 0x00 || id[2] != 0x00 || 
       id[3] != 0x0D || id[4] != 0x5D)
    {
        LOG_E("ID match failed, normal ID should be 0x0000000D5DXXXXXX."); 
        return (-RT_ERROR); 
    }

    return RT_EOK; 
}

rt_err_t stm32_psram_read_page(rt_uint8_t *buffer, uint32_t addr, uint32_t size)
{
    QSPI_CommandTypeDef cmd;

    cmd.Instruction        = PSRAM_IPS6404L_CMD_FAST_READ_QUAD;
    cmd.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode        = QSPI_ADDRESS_4_LINES;  
    cmd.AddressSize        = QSPI_ADDRESS_24_BITS;
    cmd.Address            = addr;               
    cmd.DataMode           = QSPI_DATA_4_LINES; 
    cmd.NbData             = size; 
    cmd.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;             
    cmd.AlternateBytes     = 0x00;      
    cmd.DummyCycles        = 6;
    cmd.DdrMode            = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("PSRAM_IPS6404L_CMD_FAST_READ_QUAD send failed."); 
        return (-RT_ERROR);
    }

    if(HAL_QSPI_Receive(&hqspi, buffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("stm32 psram read page failed."); 
        return (-RT_ERROR);
    }

    return RT_EOK; 
}

rt_err_t stm32_psram_write_page(rt_uint8_t *buffer, uint32_t addr, uint32_t size)
{
    QSPI_CommandTypeDef cmd;

    cmd.Instruction        = PSRAM_IPS6404L_CMD_WRITE_QUAD;
    cmd.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode        = QSPI_ADDRESS_4_LINES;  
    cmd.AddressSize        = QSPI_ADDRESS_24_BITS;
    cmd.Address            = addr;               
    cmd.DataMode           = QSPI_DATA_4_LINES; 
    cmd.NbData             = size; 
    cmd.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;
    cmd.AlternateBytesSize = 0;             
    cmd.AlternateBytes     = 0x00;      
    cmd.DummyCycles        = 0;
    cmd.DdrMode            = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("PSRAM_IPS6404L_CMD_WRITE_QUAD send failed."); 
        return (-RT_ERROR);
    }

    if(HAL_QSPI_Transmit(&hqspi, buffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        LOG_E("stm32 psram write page failed."); 
        return (-RT_ERROR);
    }

    return RT_EOK; 
}

int rt_hw_qspi_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    static DMA_HandleTypeDef hdma;  

    /* Configure peripheral GPIO */
    // PB02     ------> QUADSPI_CLK
    // PE07     ------> QUADSPI_BK2_IO0
    // PE08     ------> QUADSPI_BK2_IO1
    // PE09     ------> QUADSPI_BK2_IO2
    // PE10     ------> QUADSPI_BK2_IO3
    // PC11     ------> QUADSPI_BK2_NCS 

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
  
    GPIO_InitStruct.Pin       = GPIO_PIN_2;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_11;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* QSPI INIT */ 
    __HAL_RCC_QSPI_CLK_ENABLE(); 

    hqspi.Instance                   = QUADSPI;
    hqspi.Init.ClockPrescaler        = 1; /* QSPI CLK = HCLK3 = 200MHZ / (1+1) = 100MHz */ 
    hqspi.Init.FifoThreshold         = 4;
    hqspi.Init.SampleShifting        = QSPI_SAMPLE_SHIFTING_NONE;
    hqspi.Init.FlashSize             = 21;
    hqspi.Init.ChipSelectHighTime    = QSPI_CS_HIGH_TIME_5_CYCLE;
    hqspi.Init.ClockMode             = QSPI_CLOCK_MODE_3;
    hqspi.Init.FlashID               = QSPI_FLASH_ID_2;
    hqspi.Init.DualFlash             = QSPI_DUALFLASH_DISABLE;

    if(HAL_QSPI_Init(&hqspi) != HAL_OK)
    {
        LOG_E("qspi init failed."); 
        return (-RT_ERROR);
    }

    rt_thread_mdelay(100); 

    if(stm32_psram_reset() != RT_EOK)
    {
        LOG_E("psram reset failed."); 
        return (-RT_ERROR);
    } 

    rt_thread_mdelay(100);

    if(stm32_psram_check() != RT_EOK)
    {
        LOG_E("psram check failed."); 
        return (-RT_ERROR);
    }        
    
    LOG_I("stm32_psram_init."); 

    return RT_EOK; 
}
INIT_DEVICE_EXPORT(rt_hw_qspi_init);

// QSPI的时钟源: 
