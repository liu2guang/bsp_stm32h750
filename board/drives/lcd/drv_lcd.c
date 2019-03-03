/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2018-08-14    flybreak           the first version
 * 2018-09-18    balanceTWK         add sleep mode function
 * 2018-09-27    ZYLX               optimized display speed
 */

#include <rtdevice.h>
#include "drv_spi.h"
#include "drv_lcd.h"
#include "dfs_posix.h"

#define DBG_ENABLE 
#define DBG_SECTION_NAME    "drv.lcd"
#define DBG_COLOR
#define DBG_LEVEL           DBG_LOG
#include <rtdbg.h>

#define LCD_PWR_PIN (84)
#define LCD_DCx_PIN (83)
#define LCD_RES_PIN (81)
#define LCD_CSx_PIN (82)
#define LCD_CLEAR_SEND_NUMBER (11520)
 
static struct rt_spi_device *spi_dev_lcd;

static rt_err_t lcd_write_cmd(const rt_uint8_t cmd)
{
    rt_size_t len;

    rt_pin_write(LCD_DCx_PIN, PIN_LOW);

    len = rt_spi_send(spi_dev_lcd, &cmd, 1);

    if (len != 1)
    {
        LOG_E("lcd_write_cmd error. %d", len);
        return -RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}

static rt_err_t lcd_write_dat(const rt_uint8_t data)
{
    rt_size_t len;

    rt_pin_write(LCD_DCx_PIN, PIN_HIGH);

    len = rt_spi_send(spi_dev_lcd, &data, 1);

    if (len != 1)
    {
        LOG_E("lcd_write_data error. %d", len);
        return -RT_ERROR;
    }
    else
    {
        return RT_EOK;
    }
}

//static rt_err_t lcd_write_color(const rt_uint16_t color)
//{
//    rt_uint8_t p[2] = {0}; 

//    rt_pin_write(LCD_DCx_PIN, PIN_HIGH); 

//    p[0] = (color & 0xFF00) >> 8; 
//    p[1] = (color & 0x00FF) >> 0; 

//    if (rt_spi_send(spi_dev_lcd, (rt_uint8_t *)p, 2) != 2)
//    {
//        LOG_I("lcd_write_data error");
//        return -RT_ERROR;
//    }
//    else
//    {
//        return RT_EOK;
//    }
//}

void lcd_address_set(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2)
{
    lcd_write_cmd(0x2a);
    lcd_write_dat(x1 >> 8);
    lcd_write_dat(x1);
    lcd_write_dat(x2 >> 8);
    lcd_write_dat(x2);

    lcd_write_cmd(0x2b);
    lcd_write_dat(y1 >> 8);
    lcd_write_dat(y1);
    lcd_write_dat(y2 >> 8);
    lcd_write_dat(y2);

    lcd_write_cmd(0x2C);
}

void lcd_clear(rt_uint16_t color)
{
    rt_uint16_t i, j;
    rt_uint8_t data[2] = {0};
    rt_uint8_t *buf = RT_NULL;

    data[0] = color >> 8;
    data[1] = color;
    lcd_address_set(0, 0, 239, 239);

    /* 5760 = 240*240/20 */
    buf = rt_malloc(LCD_CLEAR_SEND_NUMBER);
    if (buf)
    {
        /* 2880 = 5760/2 color is 16 bit */
        for (j = 0; j < LCD_CLEAR_SEND_NUMBER / 2; j++)
        {
            buf[j * 2] =  data[0];
            buf[j * 2 + 1] =  data[1];
        }

        rt_pin_write(LCD_DCx_PIN, PIN_HIGH);
        for (i = 0; i < 20; i++)
        {
            rt_spi_send(spi_dev_lcd, buf, LCD_CLEAR_SEND_NUMBER);
        }
        rt_free(buf);
    }
    else
    {
        rt_pin_write(LCD_DCx_PIN, PIN_HIGH);
        for (i = 0; i < 240; i++)
        {
            for (j = 0; j < 240; j++)
            {
                rt_spi_send(spi_dev_lcd, data, 2);
            }
        }
    }
}

void lcd_showimage(const char *path) 
{
    rt_uint8_t *buf = RT_NULL; 

    int fd = open(path, O_RDONLY | O_BINARY); 
    
    lcd_address_set(0, 0, 239, 239);

    rt_uint8_t tick = rt_tick_get(); 
    
    /* 5760 = 240*240/20 */
    buf = rt_malloc(240*240*2);
    if (buf)
    {
        read(fd, buf, 240*240*2); 
        rt_pin_write(LCD_DCx_PIN, PIN_HIGH);

        for(int i = 0; i < 10; i++)
        {
            rt_spi_send(spi_dev_lcd, buf+(i*240*24*2), 240*24*2);
        }
        
        close(fd); 
        free(buf); 
    }
    else
    {
        LOG_E("lcd_showimage rt_malloc failed."); 
    }
    
    tick = rt_tick_get() - tick;
    LOG_D("1 frame is %dms", tick); 
}

int rt_hw_lcd_init(void)
{
    stm32_spi_bus_attach_device("spi3", "spi30", LCD_CSx_PIN);
    
    spi_dev_lcd = (struct rt_spi_device *)rt_device_find("spi30");

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
        cfg.max_hz = 100 * 1000 * 1000; 

        rt_spi_configure(spi_dev_lcd, &cfg);
    }
    
    rt_pin_mode(LCD_DCx_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_RES_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_PWR_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_CSx_PIN, PIN_MODE_OUTPUT);
    
    rt_pin_write(LCD_CSx_PIN, PIN_LOW);
    rt_pin_write(LCD_PWR_PIN, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(LCD_PWR_PIN, PIN_HIGH);
    rt_thread_mdelay(100);

    rt_pin_write(LCD_RES_PIN, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(LCD_RES_PIN, PIN_HIGH);
    rt_thread_mdelay(100);
    
    rt_exit_critical(); 
    {
        /* display off */
        lcd_write_cmd(0x28); 
        
        /* Memory Data Access Control */
        lcd_write_cmd(0x36); 
        lcd_write_dat(0x00);

        /* RGB 5-6-5-bit  */
        lcd_write_cmd(0x3A); 
        lcd_write_dat(0x65);

        /* Porch Setting */
        lcd_write_cmd(0xB2);
        lcd_write_dat(0x0C);
        lcd_write_dat(0x0C);
        lcd_write_dat(0x00);
        lcd_write_dat(0x33);
        lcd_write_dat(0x33);

        /* Gate Control */
        lcd_write_cmd(0xB7); 
        lcd_write_dat(0x35);  

        /* VCOM Setting */
        lcd_write_cmd(0xBB);
        lcd_write_dat(0x19);

        /* LCM Control */
        lcd_write_cmd(0xC0);
        lcd_write_dat(0x2C);

        /* VDV and VRH Command Enable */
        lcd_write_cmd(0xC2);
        lcd_write_dat(0x01);

        /* VRH Set */
        lcd_write_cmd(0xC3);
        lcd_write_dat(0x12);   

        /* VDV Set */
        lcd_write_cmd(0xC4);
        lcd_write_dat(0x20);  

        /* Frame Rate Control in Normal Mode */
        lcd_write_cmd(0xC6); 
        lcd_write_dat(0x0F);    

        /* Power Control 1 */
        lcd_write_cmd(0xD0); 
        lcd_write_dat(0xA4);
        lcd_write_dat(0xA1);

        /* Display Inversion On */
        lcd_write_cmd(0x21); 

        /* Sleep Out */
        lcd_write_cmd(0x11); 
    }
    rt_exit_critical(); 
    
    /* display on */
    lcd_write_cmd(0x29); 

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_lcd_init); 
