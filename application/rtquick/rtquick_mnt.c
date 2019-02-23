/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rtthread.h" 
#include "rtdevice.h" 

#if defined(RT_USING_DFS)
#include <dfs_file.h>
#include <dfs_posix.h>
#endif

rt_err_t rtquick_mnt_init(void)
{
    rt_err_t ret = RT_EOK; 

    extern struct romfs_dirent romfs_root;
    dfs_mount(RT_NULL, "/", "rom", 0, &romfs_root); 

#if defined(BSP_SDCARD_ENABLE_AUTO_MOUNT)
    if(dfs_mount("sd0", BSP_SDCARD_CONFIG_MOUNT_POINT, "elm", 0, 0) != 0)
    {
        rt_kprintf("sd0 mount '%s' failed.\n", BSP_SDCARD_CONFIG_MOUNT_POINT); 
    }
    
    /* 切换目录到SD卡 */ 
    extern int chdir(const char *path); 
    chdir(BSP_SDCARD_CONFIG_MOUNT_POINT); 
#endif

    return ret; 
}
