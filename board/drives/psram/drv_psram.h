/*
 * Copyright (c) 2019-2019, liu2guang
 *
 * "Anti 996" License Version 1.0 (Draft) 
 */ 

#ifndef __DRV_PSRAM_H__
#define __DRV_PSRAM_H__

#include "rtthread.h"

extern int rt_hw_psram_init(void); 
rt_err_t stm32_psram_read_page(rt_uint8_t *buffer, uint32_t addr, uint32_t size); 
rt_err_t stm32_psram_write_page(rt_uint8_t *buffer, uint32_t addr, uint32_t size); 

#endif 
