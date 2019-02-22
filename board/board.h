/*
 * Copyright (c) 2018-2019, liu2guang Personal 
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

/* STM32F469NI-Discovery Board drivers */ 

/* STM32F469NI-Discovery Board pin number */ 
#define STM32_PIN_NUM   (100)

/* STM32F469NI-Discovery Board Memory Map */ 

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

#define BOARD_SRAM_BEGIN (void *)(0x20000000u)
#define BOARD_SRAM_END   (void *)(0x20020000u)
#define BOARD_SRAM_SIZE  (((rt_uint32_t)(BOARD_SRAM_END))-((rt_uint32_t)(BOARD_SRAM_BEGIN)))

//#define BOARD_DRAM_BEGIN (void *)(0xC0000000u)
//#define BOARD_DRAM_END   (void *)(0xC0800000u)
//#define BOARD_DRAM_SIZE  (((rt_uint32_t)(BOARD_DRAM_END))-((rt_uint32_t)(BOARD_DRAM_BEGIN)))

#define BOARD_HEAP_BEGIN (void *)(HEAP_BEGIN)
#define BOARD_HEAP_END   (void *)(BOARD_SRAM_END)
#define BOARD_HEAP_SIZE  (((rt_uint32_t)(BOARD_HEAP_END))-((rt_uint32_t)(BOARD_HEAP_BEGIN)))

#endif

