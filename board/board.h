/*
 * Copyright (c) 2018-2019, liu2guang Personal 
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

/* RTTGameBoard drivers */ 
#if defined(BSP_ENABLE_PIN)
#include "drv_pin.h"
#endif 

#if defined(BSP_ENABLE_UART)
#include "drv_usart.h"
#endif 

#if defined(BSP_ENABLE_SDCARD)
#include "drv_sdcard.h"
#endif 

#if defined(BSP_ENABLE_SPI)
#include "drv_spi.h"
#endif 

#if defined(BSP_ENABLE_LCD)
#include "drv_lcd.h"
#endif 

#if defined(BSP_ENABLE_WIFI)
#include "drv_wifi.h"
#endif 

#if defined(BSP_ENABLE_PSRAM)
#include "drv_psram.h"
#endif 

/* RTTGameBoard pin number */ 
#define STM32_PIN_NUM   (100)

#define LED01 (69)
#define LED02 (68)
#define LED03 (67)
#define LED04 (66)
#define LED05 (65)

/* RTTGameBoard Memory Map */ 

#if defined(__CC_ARM) || defined(__CLANG_ARM)
    extern int Image$$RW_IRAM1$$ZI$$Limit;
    #define HEAP_BEGIN ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
    #pragma section="HEAP"
    #define HEAP_BEGIN (__segment_end("HEAP"))
#else
    extern int __bss_end;
    #define HEAP_BEGIN ((void *)&__bss_end)
#endif

#define BOARD_SRAM1_BEGIN (void *)(0x20000000u)
#define BOARD_SRAM1_END   (void *)(0x20020000u)
#define BOARD_SRAM1_SIZE  (((rt_uint32_t)(BOARD_SRAM1_END))-((rt_uint32_t)(BOARD_SRAM1_BEGIN)))

#define BOARD_SRAM2_BEGIN (void *)(0x24000000u)
#define BOARD_SRAM2_END   (void *)(0x24080000u)
#define BOARD_SRAM2_SIZE  (((rt_uint32_t)(BOARD_SRAM2_END))-((rt_uint32_t)(BOARD_SRAM2_BEGIN)))

#define BOARD_PSRAM_BEGIN (void *)(0xC0000000u)
#define BOARD_PSRAM_END   (void *)(0xC0800000u)
#define BOARD_PSRAM_SIZE  (((rt_uint32_t)(BOARD_PSRAM_END))-((rt_uint32_t)(BOARD_PSRAM_BEGIN)))

#define BOARD_HEAP_BEGIN  (void *)(HEAP_BEGIN)
#define BOARD_HEAP_END    (void *)(BOARD_SRAM1_END)
#define BOARD_HEAP_SIZE   (((rt_uint32_t)(BOARD_HEAP_END))-((rt_uint32_t)(BOARD_HEAP_BEGIN)))

#endif

