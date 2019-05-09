/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RT_QUICK_H_ 
#define __RT_QUICK_H_ 

#include "rtthread.h" 
#include "rtdevice.h" 

extern rt_err_t rtquick_init(void); 
extern void hexdump(const rt_uint8_t *p, rt_size_t len); 

#endif
