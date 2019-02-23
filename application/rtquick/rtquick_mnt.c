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

    return ret; 
}
