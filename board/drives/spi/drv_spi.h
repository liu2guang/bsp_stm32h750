/*
 * @File:   drv_spi.h 
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
 
#ifndef __DRV_SPI_H_
#define __DRV_SPI_H_

#include "rtthread.h"
#include "rtdevice.h"

rt_err_t stm32_spi_bus_attach_device(const char *bus_name, const char *device_name, rt_uint32_t pin); 

#endif 
