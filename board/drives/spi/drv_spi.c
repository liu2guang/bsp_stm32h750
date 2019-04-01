/*
 * @File:   drv_sdcard.c 
 * @Author: liuguang 
 * @Date:   2019-02-24 23:48:19 
 * 
 * @LICENSE: 
 * The code owner is liuguang. Other personnel may 
 * not be used or modified.
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2019-02-24     liuguang     The first version. 
 */ 
 
#include "drv_spi.h" 
#include "board.h" 
#include "rtdevice.h"

// #define DBG_ENABLE 
#define DBG_SECTION_NAME "drv.spi"
#define DBG_LEVEL        DBG_LOG
#define DBG_COLOR
#include <rtdbg.h> 

struct stm32_spi
{
    SPI_HandleTypeDef hspi;
    struct rt_spi_configuration *cfg;
};

struct stm32_spi_cs
{
    rt_uint32_t pin;
};

#if defined(BSP_SPI_ENABLE_PORT3)
static struct stm32_spi spi3 = {.hspi.Instance = SPI3}; 
static struct rt_spi_bus spi3_bus = {.parent.user_data = &spi3}; 
#endif

#if defined(BSP_SPI_ENABLE_PORT4)
static struct stm32_spi spi4 = {.hspi.Instance = SPI4}; 
static struct rt_spi_bus spi4_bus = {.parent.user_data = &spi4}; 
#endif

static rt_uint32_t get_spi_clk_source_freq(SPI_HandleTypeDef *hspi)
{
    rt_uint32_t freq = 0;

#if defined(BSP_SPI_ENABLE_PORT3)
    if(hspi->Instance == SPI3)
    {
        PLL1_ClocksTypeDef pll1 = {0}; 
        
        HAL_RCCEx_GetPLL1ClockFreq(&pll1); 
        freq = (rt_uint32_t)pll1.PLL1_Q_Frequency; 
    }
#endif

#if defined(BSP_SPI_ENABLE_PORT4)
    if(hspi->Instance == SPI4)
    {
        PLL2_ClocksTypeDef pll2 = {0}; 
        
        HAL_RCCEx_GetPLL2ClockFreq(&pll2); 
        freq = (rt_uint32_t)pll2.PLL2_Q_Frequency; 
    }
#endif 

    return freq; 
}

static rt_err_t spi_init(SPI_HandleTypeDef *hspi, struct rt_spi_configuration *cfg)
{
    rt_uint32_t freq = 0; 
    rt_uint32_t divx = 0;
    rt_uint32_t max_hz = 0;    

    RT_ASSERT(hspi != RT_NULL); 
    RT_ASSERT(cfg != RT_NULL); 

    if((cfg->data_width >= 4) && (cfg->data_width <= 32))
    {
        LOG_D("SPI_DATASIZE_8BIT"); 
        hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    }
    else
    {
        LOG_E("SPI data width error."); 
        return (-RT_EINVAL);    
    }

    /* 计算能配置的最接近(不高于)于目标时钟频率的分频数 */ 
    max_hz = cfg->max_hz;  
    freq = get_spi_clk_source_freq(hspi);

    if(max_hz >= freq/4)
    {
        divx = 4; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    }
    else if(max_hz >= freq/8)
    {
        divx = 8; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    }
    else if(max_hz >= freq/16)
    {
        divx = 16; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    }
    else if(max_hz >= freq/32)
    {
        divx = 32; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    }
    else if(max_hz >= freq/64)
    {
        divx = 64; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    }
    else if(max_hz >= freq/128)
    {
        divx = 128; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    }
    else
    {
        divx = 256; 
        hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    }

    LOG_D("spi clk source freq is %dHz.", freq); 
    LOG_D("spi clk target freq is %dHz.", cfg->max_hz); 
    LOG_D("spi clk divx is %d.", divx); 
    LOG_D("spi clk freq is %dHz.", freq / divx);

    /* CPOL */
    if(cfg->mode & RT_SPI_CPOL)
    {
        LOG_D("SPI_POLARITY_HIGH"); 
        hspi->Init.CLKPolarity = SPI_POLARITY_HIGH;
    }
    else
    {
        LOG_D("SPI_POLARITY_LOW"); 
        hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    } 

    /* CPHA */
    if(cfg->mode & RT_SPI_CPHA)
    {
        LOG_D("SPI_PHASE_2EDGE"); 
		hspi->Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    else
    {
        LOG_D("SPI_PHASE_1EDGE"); 
		hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
    }

    /* MSB or LSB */
    if(cfg->mode & RT_SPI_MSB)
    {
        LOG_D("SPI_FIRSTBIT_MSB"); 
        hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    }
    else
    {
        LOG_D("SPI_FIRSTBIT_LSB"); 
        hspi->Init.FirstBit = SPI_FIRSTBIT_LSB;
    }

    /* SPI3强制为3线模式3WIRE */ 
    if(hspi->Instance == SPI3)
    {
        LOG_D("SPI_DIRECTION_2LINES_TXONLY"); 
        hspi->Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
    }
    else
    {
        LOG_D("SPI_DIRECTION_2LINES"); 
        hspi->Init.Direction = SPI_DIRECTION_2LINES;
    }

    /* other */ 
    hspi->Init.Mode                       = SPI_MODE_MASTER;
    hspi->Init.NSS                        = SPI_NSS_SOFT;
    hspi->Init.NSSPMode                   = SPI_NSS_PULSE_DISABLE;
    hspi->Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
    hspi->Init.TIMode                     = SPI_TIMODE_DISABLE;
    hspi->Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
    hspi->Init.CRCPolynomial              = 7;
    hspi->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi->Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi->Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi->Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi->Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi->Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
    hspi->Init.FifoThreshold              = SPI_FIFO_THRESHOLD_08DATA;
    
    if (HAL_SPI_Init(hspi) != HAL_OK)
    {
        return (-RT_ERROR); 
    }
    
    LOG_D("spi configuration."); 

    return RT_EOK; 
}

static rt_err_t configure(struct rt_spi_device *device, struct rt_spi_configuration *cfg)
{
    rt_err_t ret = RT_EOK; 

    struct stm32_spi *hspi = (struct stm32_spi *)device->bus->parent.user_data;
    hspi->cfg = cfg;

    ret = spi_init(&(hspi->hspi), cfg);

    return ret; 
}

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    HAL_StatusTypeDef ret; 
    rt_uint32_t length = message->length; 
    
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);

    struct stm32_spi *hspi = (struct stm32_spi *)device->bus->parent.user_data;
    struct stm32_spi_cs *cs = device->parent.user_data;

    if (message->cs_take)
    {
        rt_pin_write(cs->pin, PIN_LOW);
    }
    
    if(message->length == 0)
    {
        length = 0; 
        goto _ret; 
    }

    if(message->send_buf == RT_NULL && message->recv_buf == RT_NULL)
    {
        LOG_E("send_buf and recv_buf is null."); 
        while(1); 
    }
    else if(message->send_buf != RT_NULL && message->recv_buf == RT_NULL)
    {
        ret = HAL_SPI_Transmit(&(hspi->hspi), (uint8_t *)(message->send_buf), message->length, 1000); 
        if(ret != HAL_OK)
        {
            LOG_D("HAL_SPI_Transmit = failed.", ret); 
            while(1); 
        }
    }
    else if(message->send_buf == RT_NULL && message->recv_buf != RT_NULL)
    {
        ret = HAL_SPI_Receive(&(hspi->hspi), (uint8_t *)(message->recv_buf), message->length, 1000); 
        if(ret != HAL_OK)
        {
            LOG_D("HAL_SPI_Receive = failed.", ret); 
            while(1); 
        }
    }
    else if(message->send_buf != RT_NULL && message->recv_buf != RT_NULL)
    {
        ret = HAL_SPI_TransmitReceive(&(hspi->hspi), (uint8_t *)(message->send_buf), (uint8_t *)(message->recv_buf), message->length, 1000); 
        if(ret != HAL_OK)
        {
            LOG_D("HAL_SPI_TransmitReceive = failed.", ret); 
            while(1); 
        }
    }

    while (HAL_SPI_GetState(&(hspi->hspi)) != HAL_SPI_STATE_READY);
    
_ret:
    if (message->cs_release)
    {
        rt_pin_write(cs->pin, PIN_HIGH);
    }

    return length;
}

rt_err_t stm32_spi_bus_attach_device(const char *bus_name, const char *device_name, rt_uint32_t pin)
{
    rt_err_t ret = RT_EOK;
    
    struct rt_spi_device *spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device)); 
    RT_ASSERT(spi_device != RT_NULL); 

    struct stm32_spi_cs *cs_pin = (struct stm32_spi_cs *)rt_malloc(sizeof(struct stm32_spi_cs));
    RT_ASSERT(cs_pin != RT_NULL);

    cs_pin->pin = pin;
    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, PIN_HIGH); 
 
    ret = rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)cs_pin); 
    
    return ret; 
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    LOG_D("HAL_SPI_MspInit"); 
   
#if defined(BSP_SPI_ENABLE_PORT3)     
    if(hspi->Instance == SPI3)
    {
        __HAL_RCC_SPI3_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();

        GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  
    }
#endif
    
#if defined(BSP_SPI_ENABLE_PORT4)   
    if(hspi->Instance == SPI4)
    {
        __HAL_RCC_SPI4_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();

        GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    }
#endif
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
#if defined(BSP_SPI_ENABLE_PORT3)     
    if(hspi->Instance == SPI3)
    {
        __HAL_RCC_SPI3_CLK_DISABLE(); 
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_12);  
    }
#endif
    
#if defined(BSP_SPI_ENABLE_PORT4)   
    if(hspi->Instance == SPI4)
    {
        __HAL_RCC_SPI4_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOE, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);  
    }
#endif 
}

static struct rt_spi_ops ops = {configure, spixfer}; 

int rt_hw_spi_init(void)
{
#if defined(BSP_SPI_ENABLE_PORT3) 
    rt_spi_bus_register(&spi3_bus, "spi3", &ops); 
#endif

#if defined(BSP_SPI_ENABLE_PORT4)
    rt_spi_bus_register(&spi4_bus, "spi4", &ops); 
#endif

    return RT_EOK; 
}
INIT_DEVICE_EXPORT(rt_hw_spi_init);
