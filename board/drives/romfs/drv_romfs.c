/* Generated by mkromfs. Edit with caution. */
#include <rtthread.h>
#include <dfs_romfs.h>



static const struct romfs_dirent _romfs_root_dev[] = {
    {ROMFS_DIRENT_FILE, "null", RT_NULL, 0}
};

static const rt_uint8_t _romfs_root_etc_bsp_LICENSE[] = {
0x23,0x20,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x0d,0x0a,0x23,0x20,0x40,0x46,0x69,0x6c,0x65,0x3a,0x20,0x20,0x20,0x7a,0x2e,0x62,0x73,0x70,0x2e,0x6c,0x69,0x6e,0x6b,0x2e,0x74,0x78,0x74,0x0d,0x0a,0x23,0x20,0x40,0x41,0x75,0x74,0x68,0x6f,0x72,0x3a,0x20,0x6c,0x69,0x75,0x32,0x67,0x75,0x61,0x6e,0x67,0x0d,0x0a,0x23,0x20,0x40,0x44,0x61,0x74,0x65,0x3a,0x20,0x20,0x20,0x32,0x30,0x31,0x39,0x2d,0x30,0x32,0x2d,0x31,0x31,0x20,0x31,0x36,0x3a,0x32,0x39,0x3a,0x30,0x30,0x0d,0x0a,0x23,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x5f,0x5f,0x5f,0x5f,0x5f,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x2f,0x20,0x20,0x20,0x20,0x20,0x5c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x76,0x76,0x76,0x76,0x76,0x76,0x76,0x20,0x20,0x2f,0x7c,0x5f,0x5f,0x2f,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x49,0x20,0x20,0x20,0x2f,0x4f,0x2c,0x4f,0x20,0x20,0x20,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x49,0x20,0x2f,0x5f,0x5f,0x5f,0x5f,0x5f,0x20,0x20,0x20,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x2f,0x7c,0x2f,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x4a,0x7c,0x2f,0x5e,0x20,0x5e,0x20,0x5e,0x20,0x5c,0x20,0x20,0x7c,0x20,0x20,0x20,0x20,0x2f,0x30,0x30,0x20,0x20,0x7c,0x20,0x20,0x20,0x20,0x5f,0x2f,0x2f,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7c,0x5e,0x20,0x5e,0x20,0x5e,0x20,0x5e,0x20,0x7c,0x57,0x7c,0x20,0x20,0x20,0x7c,0x2f,0x5e,0x5e,0x5c,0x20,0x7c,0x20,0x20,0x20,0x2f,0x6f,0x6f,0x20,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x5c,0x6d,0x5f,0x5f,0x5f,0x6d,0x5f,0x5f,0x7c,0x5f,0x7c,0x20,0x20,0x20,0x20,0x5c,0x6d,0x5f,0x6d,0x5f,0x7c,0x20,0x20,0x20,0x5c,0x6d,0x6d,0x5f,0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x54,0x65,0x73,0x74,0x20,0x77,0x69,0x74,0x68,0x20,0x42,0x53,0x50,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x2d,0x2d,0x2d,0x20,0x6c,0x69,0x75,0x32,0x67,0x75,0x61,0x6e,0x67,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0d,0x0a,0x23,0x0d,0x0a,0x23,0x20,0x40,0x41,0x75,0x74,0x68,0x6f,0x72,0x20,0x20,0x20,0x20,0x20,0x20,0x3a,0x20,0x68,0x74,0x74,0x70,0x73,0x3a,0x2f,0x2f,0x67,0x69,0x74,0x68,0x75,0x62,0x2e,0x63,0x6f,0x6d,0x2f,0x6c,0x69,0x75,0x32,0x67,0x75,0x61,0x6e,0x67,0x0d,0x0a,0x23,0x20,0x40,0x52,0x65,0x70,0x6f,0x73,0x69,0x74,0x6f,0x72,0x79,0x20,0x20,0x3a,0x20,0x68,0x74,0x74,0x70,0x73,0x3a,0x2f,0x2f,0x67,0x69,0x74,0x68,0x75,0x62,0x2e,0x63,0x6f,0x6d,0x2f,0x6c,0x69,0x75,0x32,0x67,0x75,0x61,0x6e,0x67,0x2f,0x62,0x73,0x70,0x5f,0x73,0x74,0x6d,0x33,0x32,0x66,0x34,0x36,0x39,0x0d,0x0a,0x23,0x20,0x40,0x4c,0x49,0x43,0x45,0x4e,0x53,0x45,0x3a,0x20,0x4d,0x49,0x54,0x3a,0x20,0x68,0x74,0x74,0x70,0x73,0x3a,0x2f,0x2f,0x67,0x69,0x74,0x68,0x75,0x62,0x2e,0x63,0x6f,0x6d,0x2f,0x6c,0x69,0x75,0x32,0x67,0x75,0x61,0x6e,0x67,0x2f,0x62,0x73,0x70,0x5f,0x73,0x74,0x6d,0x33,0x32,0x66,0x34,0x36,0x39,0x2f,0x62,0x6c,0x6f,0x62,0x2f,0x6d,0x61,0x73,0x74,0x65,0x72,0x2f,0x4c,0x49,0x43,0x45,0x4e,0x53,0x45,0x2e,0x0d,0x0a,0x23,0x0d,0x0a,0x23,0x20,0x43,0x68,0x61,0x6e,0x67,0x65,0x20,0x4c,0x6f,0x67,0x73,0x3a,0x0d,0x0a,0x23,0x20,0x44,0x61,0x74,0x65,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x41,0x75,0x74,0x68,0x6f,0x72,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x4e,0x6f,0x74,0x65,0x73,0x20,0x0d,0x0a,0x23,0x20,0x32,0x30,0x31,0x39,0x2d,0x30,0x32,0x2d,0x31,0x31,0x20,0x20,0x20,0x20,0x20,0x6c,0x69,0x75,0x32,0x67,0x75,0x61,0x6e,0x67,0x20,0x20,0x20,0x20,0x54,0x68,0x65,0x20,0x66,0x69,0x72,0x73,0x74,0x20,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x2e,0x20,0x0d,0x0a,0x23,0x0d,0x0a,0x23,0x20,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x0d,0x0a
};

static const struct romfs_dirent _romfs_root_etc[] = {
    {ROMFS_DIRENT_FILE, "bsp.LICENSE", (rt_uint8_t *)_romfs_root_etc_bsp_LICENSE, sizeof(_romfs_root_etc_bsp_LICENSE)/sizeof(_romfs_root_etc_bsp_LICENSE[0])}
};



static const struct romfs_dirent _romfs_root_mnt_flash[] = {
    {ROMFS_DIRENT_FILE, "null", RT_NULL, 0}
};



static const struct romfs_dirent _romfs_root_mnt_sdcard[] = {
    {ROMFS_DIRENT_FILE, "null", RT_NULL, 0}
};



static const struct romfs_dirent _romfs_root_mnt_sdram[] = {
    {ROMFS_DIRENT_FILE, "null", RT_NULL, 0}
};



static const struct romfs_dirent _romfs_root_mnt_usbdisk[] = {
    {ROMFS_DIRENT_FILE, "null", RT_NULL, 0}
};

static const struct romfs_dirent _romfs_root_mnt[] = {
    {ROMFS_DIRENT_DIR, "flash", (rt_uint8_t *)_romfs_root_mnt_flash, sizeof(_romfs_root_mnt_flash)/sizeof(_romfs_root_mnt_flash[0])},
    {ROMFS_DIRENT_DIR, "sdcard", (rt_uint8_t *)_romfs_root_mnt_sdcard, sizeof(_romfs_root_mnt_sdcard)/sizeof(_romfs_root_mnt_sdcard[0])},
    {ROMFS_DIRENT_DIR, "sdram", (rt_uint8_t *)_romfs_root_mnt_sdram, sizeof(_romfs_root_mnt_sdram)/sizeof(_romfs_root_mnt_sdram[0])},
    {ROMFS_DIRENT_DIR, "usbdisk", (rt_uint8_t *)_romfs_root_mnt_usbdisk, sizeof(_romfs_root_mnt_usbdisk)/sizeof(_romfs_root_mnt_usbdisk[0])}
};

static const struct romfs_dirent _romfs_root[] = {
    {ROMFS_DIRENT_DIR, "dev", (rt_uint8_t *)_romfs_root_dev, sizeof(_romfs_root_dev)/sizeof(_romfs_root_dev[0])},
    {ROMFS_DIRENT_DIR, "etc", (rt_uint8_t *)_romfs_root_etc, sizeof(_romfs_root_etc)/sizeof(_romfs_root_etc[0])},
    {ROMFS_DIRENT_DIR, "mnt", (rt_uint8_t *)_romfs_root_mnt, sizeof(_romfs_root_mnt)/sizeof(_romfs_root_mnt[0])}
};

const struct romfs_dirent romfs_root = {
    ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_root, sizeof(_romfs_root)/sizeof(_romfs_root[0])
};
