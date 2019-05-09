/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include "rtquick.h" 
#include "board.h" 

rt_uint8_t buff0[1024] = "Hello RT-Thread!";
rt_uint8_t buff1[1024] = {0};

int main(void)
{      
    rtquick_init(); 
     
    stm32_psram_write_page(buff0, 0x00, 32); 
    
    rt_kprintf("buff0: 0x%.8x.\n", (rt_uint32_t)buff0);  hexdump(buff0, 48); 
    
    stm32_psram_read_page(buff1, 0x00, 32);  
    
    rt_kprintf("buff1: 0x%.8x.\n", (rt_uint32_t)buff1); hexdump(buff1, 48); 
    
    return 0; 
}
