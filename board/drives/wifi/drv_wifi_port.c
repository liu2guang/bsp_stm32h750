#include <rtthread.h>

#include <rtdevice.h>
#include <drv_spi.h>
#include <board.h>
#include <drv_wifi.h>

#define RW007_AT_MODE   3
#define RW007_SPI_MODE  1

extern void spi_wifi_isr(int vector);

static void set_rw007_mode(int mode)
{
    /* Configure IO */
    rt_pin_mode(RW007_RST_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(RW007_BOOT0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(RW007_BOOT1_PIN, PIN_MODE_OUTPUT);
    
    /* Reset rw007 and config mode */
    rt_pin_write(RW007_RST_PIN, PIN_LOW);
    rt_pin_write(RW007_BOOT0_PIN, mode & 0x01 ? PIN_HIGH : PIN_LOW);
    rt_pin_write(RW007_BOOT1_PIN, mode & 0x02 ? PIN_HIGH : PIN_LOW);
    rt_thread_delay(rt_tick_from_millisecond(100));
    rt_pin_write(RW007_RST_PIN, PIN_HIGH);

    /* Wait rw007 ready(exit busy stat) */
    while(!rt_pin_read(RW007_INT_BUSY_PIN))
    {
    }
    rt_thread_delay(rt_tick_from_millisecond(100));
}

int wifi_spi_device_init(void)
{
    set_rw007_mode(RW007_SPI_MODE);

    stm32_spi_bus_attach_device(RW007_SPI_BUS_NAME, "wspi", RW007_CS_PIN); 
    rt_hw_wifi_init("wspi", MODE_STATION);
    
    return 0; 
}
INIT_APP_EXPORT(wifi_spi_device_init);
    
static void int_wifi_irq(void * p)
{
    ((void)p);
    if(rt_pin_read(RW007_INT_BUSY_PIN))
    {
        spi_wifi_isr(0);
    }
}

void spi_wifi_hw_init(void)
{
    rt_pin_attach_irq(RW007_INT_BUSY_PIN, PIN_IRQ_MODE_FALLING, int_wifi_irq, 0);
    rt_pin_irq_enable(RW007_INT_BUSY_PIN, RT_TRUE);
}

rt_bool_t spi_wifi_is_busy(void)
{
    return !rt_pin_read(RW007_INT_BUSY_PIN);
}

void spi_wifi_int_cmd(rt_bool_t cmd)
{
    rt_pin_irq_enable(RW007_INT_BUSY_PIN, cmd);
}
