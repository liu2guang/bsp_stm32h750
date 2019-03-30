/*
 * @File:   hexdump.c
 * @Author: liu2guang
 * @Date:   2018-10-12 14:56:10
 *
 * @LICENSE: MIT
 * https://github.com/rtpkgs/libtest/blob/master/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-12     liu2guang    update RT-Thread hexdump toolkis.
 */

#include "rtthread.h" 
#include "stdio.h"

void hexdump(const rt_uint8_t *p, rt_size_t len)
{
    unsigned char *buf = (unsigned char*)p;
    int i, j;
    
    rt_kprintf("Dump 0x%.8x %dBytes\n", (int)p, len); 
    rt_kprintf("Offset      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n"); 

    for (i=0; i<len; i+=16)
    {
        rt_kprintf("0x%08X: ", i+(int)p);

        for (j=0; j<16; j++)
        {
            if (i+j < len)
            {
                rt_kprintf("%02X ", buf[i+j]);
            }
            else
            {
                rt_kprintf("   ");
            }
        } 
        rt_kprintf(" ");

        for (j=0; j<16; j++)
        {
            if (i+j < len)
            {
                rt_kprintf("%c", ((unsigned int)((buf[i+j]) - ' ') < 127u - ' ') ? buf[i+j] : '.');
            }
        }
        rt_kprintf("\n");
    }
}

int ramdump(int argc, char *argv[])
{
    int addr = 0; 
    rt_size_t len = 0; 

    if(argc != 3)
    {
		rt_kprintf("example: ramdump 0x00000000 len\n");
        return -1; 
    }

    sscanf(argv[1],"%x",&addr);  
    sscanf(argv[2],"%d",&len); 

    hexdump((const rt_uint8_t *)addr, len); 
    
    return 0; 
}
MSH_CMD_EXPORT(ramdump, dump ram info.)
