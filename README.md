#  STM32H750

## 1. 简介

该 BSP 是基于 RTTGameBoard 开发板 定制开发, 具备以下简要的特性：

| 介绍 | 描述 |
| ---- | ---- |
| 主CPU平台 | ARM Cortex-M7 |
| 最高频率 | 400MHz |
| 内部存储器 | 128KB Flash 1MB RAM |
| 外部存储器 | 8MB PSRAM 16MB QSPI FLASH |

## 2. 编译说明

STM32H750VB 板级包支持MDK5开发环境和GCC编译器，以下是具体版本信息：

| IDE/编译器 | 已测试版本 |
| ---------- | --------- |
| MDK5 | MDK522 |
| GCC | GCC 5.4.1 20160919 (release) |
| IAR | 非常讨厌IAR开发环境, 需要的自己去处理 |

## 3. 使用说明

1. 该 BSP 和 RT-Thread 官方支持维护的 BSP 不同, 支持MDK编译和GCC编译, MDK编译后的文件生成 `build/rtthread_mdk.bin`, GCC编译会生成 `build/rtthread_gcc.bin`, 同时使用相互之间不会再覆盖. 
2. 该 BSP 支持快速更新 ROMFS, 只需要将你自己的 ROMFS 的目录格式写到 `board/romfs/` 目录下即可, 在ENV中使用 `scons -jx` 和 `scons --target=xxx -s` 都会自动生成 drv_romfs.c 的新驱动, 再编译后下载就可以更新 ROMFS. 
3. `scripts` 目录下的 `clear.bat` 是用于编译或者生成工程后清除垃圾中间不需要的文件, 保持 BSP 的整洁.
4. `scripts` 目录下的 `clearbsp.bat` 可以删除所有中间文件和工程文件, 这里是作者为了上传github使用, 一般用户请谨慎使用.
5. 该 BSP 对 RT-Thread 做出了魔改, 所以需要使用特定的版本RT-Thread, 这些魔改只是为了 BSP 更加简洁, 不用涉及功能上的修改, 如果克隆后无法更新 RT-Thread 子模块, 请手动给子模块添加远端: `https://github.com/liu2guang/rt-thread`.   

## 4. 驱动支持情况及计划

| 驱动 | 支持情况  |
| :------ | :----  |
| UART | 支持串口3 |
| SDCARD | 支持轮询方式, DMA暂时没调, 读7.3MB/s, 写4.5MB/s |
| romfs | 支持 |

## 5. 维护人信息
- liu2guang: [Github](https://github.com/liu2guang). 
