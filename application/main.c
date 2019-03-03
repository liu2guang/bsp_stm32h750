/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "rtquick.h" 
#include "board.h" 
#include "drv_lcd.h" 

int main(void)
{      
    rtquick_init(); 
    
    lcd_showimage("/mnt/sdcard/logo2.bin"); 
    
    return 0; 
}
