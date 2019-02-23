/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rtthread.h" 
#include "rtdevice.h" 
#include "board.h" 

int reboot(void)
{
    NVIC_SystemReset(); 
    return RT_EOK; 
}
MSH_CMD_EXPORT(reboot, reboot board.); 

rt_err_t rtquick_utils_init(void)
{
    rt_err_t ret = RT_EOK; 

    return ret; 
}
