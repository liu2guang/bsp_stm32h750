/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rtthread.h" 
#include "rtdevice.h"

#if defined(BSP_ENABLE_WIFI) 

#include "spi_wifi_rw007.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "dfs_file.h"
#include "dfs_posix.h"

struct rtquick_wifi
{
    char ssids[32][32]; 
    char passwords[32][32]; 
    
    rt_uint8_t cnt; 
};
typedef struct rtquick_wifi* rtquick_wifi_t; 

static struct rtquick_wifi wifi = {0}; 

static int search(char *path)
{
    rt_err_t ret = RT_EOK; 
    DIR *dp = NULL;
    FILE *fp = NULL;
    struct dirent *dirp = NULL;
    struct stat statbuf;
    char *fullpath = NULL; 
    char password[32]; 
    
    int len = -1; 

    // 打开指定目录
    if((dp = opendir(path)) == NULL)
    {
        rt_kprintf("open %s dir failed.\n", path); 
        ret = (-RT_ERROR); 
        goto _ret; 
    }

    // 开始遍历目录
    while((dirp = readdir(dp)) != NULL)
    {
        // 跳过'.'和'..'两个目录
        if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
        {
            continue; 
        }
        
        fullpath = dfs_normalize_path(path, dirp->d_name); 

        // 读取文件状态
        if(stat(fullpath, &statbuf) < 0)
        {
            continue; 
        }
        
        // 不是文件则跳过
        if(S_ISDIR(statbuf.st_mode) != 0)
        {
            continue; 
        }
        
        // 读取张号密码并保存
        if((fp = fopen(fullpath, "r")) != NULL)
        {
            memset(password, 0x00, sizeof(password)); 
            
            len = fread(password, 1, sizeof(password), fp); 
            if(len <= 0)
            {
                fclose(fp); 
                continue; 
            }
            
            rt_strncpy(wifi.ssids[wifi.cnt], dirp->d_name, rt_strlen(dirp->d_name)); 
            rt_strncpy(wifi.passwords[wifi.cnt], password, len); 
            
            wifi.cnt++; 
        }
        
        fclose(fp); 
    }

    closedir(dp); 

_ret:
    return ret; 
}

static int is_connected(void)
{
    ip_addr_t ip_addr;
    int result = 0;

    ip_addr_set_zero(&ip_addr);
    if (ip_addr_cmp(&(netif_list->ip_addr), &ip_addr))
    {
        result = 0;
    }
    else
    {
        result = 1;
        rt_kprintf("IP address: %s\n", ipaddr_ntoa(&(netif_list->ip_addr)));
    }

    return result;
}

rt_err_t rtquick_wifi_init(void)
{
    rt_err_t ret = RT_EOK;
    int i = 0, j = 0, cnt = 1; 
    rw007_ap_info *ap_info = RT_NULL;
    struct rw007_wifi *rw007_wifi = RT_NULL;

    search("/mnt/sdcard/etc/wifi"); 
    
    rw007_wifi = (struct rw007_wifi *)rt_device_find("w0");
    if(rw007_wifi == RT_NULL)
    {
        rt_kprintf("do not found w0 device.\n");
        ret = (-RT_ERROR); 
        goto _ret; 
    }
    
    /* 扫描WIFI */ 
    ret = rt_device_control((rt_device_t)rw007_wifi, RW007_CMD_SCAN, RT_NULL); 
    if(ret != RT_EOK)
    {
        rt_kprintf("w0 scan failed.\n");
        ret = (-RT_ERROR); 
        goto _ret; 
    }

    /* 匹配密码 */ 
    for(i = 0; i < rw007_wifi->ap_scan_count; i++) 
    {
        ap_info = &rw007_wifi->ap_scan[i]; 

        for(j = 0; j < (wifi.cnt); j++)
        {
            if(rt_strcmp(ap_info->ssid, wifi.ssids[j]) == 0) 
            {
                goto _connect; 
            }
        }
    }

    ret = (-RT_ERROR); 
    rt_kprintf("\nNo WIFI password configuration, Stop connecting to WIFI.\n"); 

    return ret; 

    /* 连接WIFI */ 
_connect:
    rt_kprintf("\nTry connect SSID: %s ...\n", wifi.ssids[j]); 
    rw007_join(wifi.ssids[j], wifi.passwords[j]); 

    while(!is_connected()) 
    {
        if(cnt++ % (50) == 0)
        {
            rt_kprintf("Try reconnecting...\n"); 
            rw007_join(wifi.ssids[j], wifi.passwords[j]); 
        }

        rt_thread_mdelay(100); 
    }
    rt_kprintf("Net connect successful!\n"); 

_ret:
    return ret; 
}

#endif 
