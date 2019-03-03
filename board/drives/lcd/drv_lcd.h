/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2018-08-14    flybreak           the first version
 * 2018-09-18    balanceTWK         add sleep mode function 
 */

#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

#include <rtthread.h>

#define LCD_W 240
#define LCD_H 240

//POINT_COLOR
#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              0xF800
#define MAGENTA 0x1FF8
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40
#define BRRED            0XFC07
#define GRAY             0X8430
#define GRAY175          0XAD75
#define GRAY151          0X94B2
#define GRAY187          0XBDD7
#define GRAY240          0XF79E

void lcd_showimage(const char *path); 

#endif
