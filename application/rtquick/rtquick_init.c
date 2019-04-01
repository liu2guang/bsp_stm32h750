/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rtthread.h" 
#include "rtdevice.h" 

extern rt_err_t rtquick_utils_init(void); 
extern rt_err_t rtquick_mnt_init(void); 
extern rt_err_t rtquick_wifi_init(void); 

rt_err_t rtquick_init(void)
{
    int ret = RT_EOK; 

    rtquick_utils_init(); 
    rtquick_mnt_init(); 
    
#if defined(BSP_ENABLE_WIFI) 
    rtquick_wifi_init(); 
#endif

    return ret; 
}
