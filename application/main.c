/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtthread.h"
#include "board.h" 

void t1(void *p)
{
    while(1)
    {
        rt_kprintf("Hello RTTGameBoard V1.0, Hello RTThread 1.\n"); 
        rt_thread_mdelay(1000); 
    }
}

void t2(void *p)
{
    while(1)
    {
        rt_kprintf("Hello RTTGameBoard V1.0, Hello RTThread 2.\n"); 
        rt_thread_mdelay(1000); 
    }
}

int main(void)
{   
    rt_thread_t thread = RT_NULL; 
    
    thread = rt_thread_create("t1", t1, RT_NULL, 1024,  9, 10); 
    rt_thread_startup(thread); 
    
    thread = rt_thread_create("t2", t2, RT_NULL, 1024, 10, 10); 
    rt_thread_startup(thread);   
    
    return 0; 
}
