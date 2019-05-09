/*
 * COPYRIGHT (C) 2018, Real-Thread Information Technology Ltd
 * 
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2014-07-31     aozima       the first version
 * 2014-09-18     aozima       update command & response.
 * 2017-07-28     armink       fix auto reconnect feature
 * 2018-12-24     zyh          porting rw007 from rw009
 */

#include <drivers/spi.h>
#include <rtthread.h>

#include "lwipopts.h"
#include <lwip/icmp.h>
#include <netif/etharp.h>
#include <netif/ethernetif.h>

#ifndef RW007_LOG_LEVEL
#define RW007_LOG_LEVEL DBG_LOG
#endif

//#define DBG_ENABLE
#define DBG_SECTION_NAME "[RW007]"
#define DBG_LEVEL RW007_LOG_LEVEL
#define DBG_COLOR
#include <rtdbg.h>

/********************************* RW007 **************************************/
#include "drv_wifi.h"

static struct rw007_wifi rw007_wifi_device;
static struct rt_event spi_wifi_data_event;

static void resp_handler(struct rw007_wifi *wifi_device, struct rw007_resp *resp)
{
    struct rw007_resp *resp_return = RT_NULL;

    switch (resp->cmd)
    {
    case RW007_CMD_INIT:
        rt_kprintf("[RW007] rw007 init done\n");
        resp_return = (struct rw007_resp *)rt_malloc(member_offset(struct rw007_resp, resp) + sizeof(rw007_resp_init)); //TODO:
        if (resp_return == RT_NULL)
            break;
        memcpy(resp_return, resp, member_offset(struct rw007_resp, resp) + sizeof(rw007_resp_init));

        rt_kprintf("[RW007] sn:%-*.*s\n", sizeof(resp->resp.init.sn), sizeof(resp->resp.init.sn), resp->resp.init.sn);
        rt_kprintf("[RW007] version:%-*.*s\n", sizeof(resp->resp.init.version), sizeof(resp->resp.init.version), resp->resp.init.version);

        rt_memcpy(wifi_device->dev_addr, resp->resp.init.mac, 6);
        break;

    case RW007_CMD_SCAN:
        if (resp->len == sizeof(rw007_ap_info))
        {
            rw007_ap_info *ap_scan = rt_realloc(wifi_device->ap_scan, sizeof(rw007_ap_info) * (wifi_device->ap_scan_count + 1));
            if (ap_scan != RT_NULL)
            {
                memcpy(&ap_scan[wifi_device->ap_scan_count], &resp->resp.ap_info, sizeof(rw007_ap_info));

#if defined(DBG_ENABLE)
                {
                    rw007_ap_info *ap_info = &resp->resp.ap_info;

                    LOG_D("SCAN SSID:%-32.32s", ap_info->ssid);
                    LOG_D("SCAN BSSID:%02X-%02X-%02X-%02X-%02X-%02X",
                          ap_info->bssid[0],
                          ap_info->bssid[1],
                          ap_info->bssid[2],
                          ap_info->bssid[3],
                          ap_info->bssid[4],
                          ap_info->bssid[5]);
                    LOG_D("SCAN rssi:%ddBm", ap_info->rssi);
                    LOG_D("SCAN rate:%dMbps", ap_info->max_data_rate / 1000);
                    LOG_D("SCAN channel:%d", ap_info->channel);
                    LOG_D("SCAN security:%08X", ap_info->security);
                }
#endif
                wifi_device->ap_scan_count++;
                wifi_device->ap_scan = ap_scan;
            }

            return; /* wait for next ap */
        }
        break;
    case RW007_CMD_JOIN:
    case RW007_CMD_EASY_JOIN:
        LOG_D("resp_handler RW007_CMD_EASY_JOIN");
        resp_return = (struct rw007_resp *)rt_malloc(member_offset(struct rw007_resp, resp) + sizeof(rw007_resp_join)); //TODO:
        if (resp_return == RT_NULL)
            break;
        memcpy(resp_return, resp, member_offset(struct rw007_resp, resp) + sizeof(rw007_resp_join));

        if (resp->result == 0)
        {
            memcpy(&wifi_device->ap_info, &resp_return->resp.ap_info, sizeof(rw007_resp_join));
            wifi_device->active = 1;
            eth_device_linkchange(&wifi_device->parent, RT_TRUE);
        }
        else
        {
            wifi_device->active = 1;
            eth_device_linkchange(&wifi_device->parent, RT_FALSE);
            LOG_I("RW007_CMD_EASY_JOIN result: %d", resp->result);
        }
        {
            rw007_ap_info *ap_info = &resp->resp.ap_info;
            wifi_device->ap_info = *ap_info;
#if defined(DBG_ENABLE)
            

            if(ap_info->channel == 0)
            {
                rt_kprintf("[RW007] WIFI Connect failed!\n");
            }
            else
            {
                rt_kprintf("[RW007] WIFI Connected!\n");
                LOG_D("JOIN SSID:%-32.32s", ap_info->ssid);
                LOG_D("JOIN BSSID:%02X-%02X-%02X-%02X-%02X-%02X",
                    ap_info->bssid[0],
                    ap_info->bssid[1],
                    ap_info->bssid[2],
                    ap_info->bssid[3],
                    ap_info->bssid[4],
                    ap_info->bssid[5]);
                LOG_D("JOIN rssi:%ddBm", ap_info->rssi);
                LOG_D("JOIN rate:%dMbps", ap_info->max_data_rate / 1000);
                LOG_D("JOIN channel:%d", ap_info->channel);
                LOG_D("JOIN security:%08X", ap_info->security);
                
            }
#endif
        }
        break;

    case RW007_CMD_RSSI:
        {
            wifi_device->ap_info.rssi = *((int *)&resp->resp);
            rt_kprintf("[RW007] current RSSI: %ddBm\n", wifi_device->ap_info.rssi);
        }
        break;

    case RW007_CMD_SOFTAP:
    {
        if (resp->result == 0)
        {
            wifi_device->active = 1;
            eth_device_linkchange(&wifi_device->parent, RT_TRUE);
        }
        else
        {
            LOG_I("RW007_CMD_EASY_JOIN result: %d", resp->result);
        }
    }
    break;

    default:
        LOG_W("unknow resp_handler %d", resp->cmd);
        break;
    }

    if (resp->cmd == wifi_device->last_cmd)
    {
        rt_mb_send(&wifi_device->rw007_cmd_mb, (rt_uint32_t)resp_return);
        return;
    }
    else
    {
        rt_free(resp_return);
    }
}

static rt_err_t rw007_cmd(struct rw007_wifi *wifi_device, uint32_t cmd, void *args)
{
    rt_err_t result = RT_EOK;
    rt_int32_t timeout = RW007_CMD_TIMEOUT;

    struct spi_data_packet *data_packet;
    struct rw007_cmd *wifi_cmd = RT_NULL;
    struct rw007_resp *resp = RT_NULL;

    wifi_device->last_cmd = cmd;

    data_packet = (struct spi_data_packet *)rt_mp_alloc(&wifi_device->spi_tx_mp, RT_WAITING_FOREVER);
    wifi_cmd = (struct rw007_cmd *)data_packet->buffer;

    wifi_cmd->cmd = cmd;
    wifi_cmd->len = 0;

    if (cmd == RW007_CMD_INIT)
    {
        wifi_cmd->len = sizeof(rw007_cmd_init);
    }
    else if (cmd == RW007_CMD_SCAN)
    {
        wifi_cmd->len = 0;
        timeout += RT_TICK_PER_SECOND * 10;

        if (wifi_device->ap_scan)
        {
            rt_free(wifi_device->ap_scan);
            wifi_device->ap_scan = RT_NULL;
            wifi_device->ap_scan_count = 0;
        }
    }
    else if (cmd == RW007_CMD_JOIN)
    {
        wifi_cmd->len = sizeof(rw007_cmd_join);
    }
    else if (cmd == RW007_CMD_EASY_JOIN)
    {
        wifi_cmd->len = sizeof(rw007_cmd_easy_join);
        timeout += RT_TICK_PER_SECOND * 5;
    }
    else if (cmd == RW007_CMD_RSSI)
    {
        wifi_cmd->len = sizeof(rw007_cmd_rssi);
    }
    else if (cmd == RW007_CMD_SOFTAP)
    {
        wifi_cmd->len = sizeof(rw007_cmd_softap);
    }
    else
    {
        LOG_E("unkown RW007 CMD %d", cmd);
        result = -RT_ENOSYS;
        rt_mp_free(data_packet);
        data_packet = RT_NULL;
    }

    if (data_packet == RT_NULL)
    {
        goto _exit;
    }

    if (wifi_cmd->len)
        memcpy(&wifi_cmd->params, args, wifi_cmd->len);

    data_packet->data_type = data_type_cmd;
    data_packet->data_len = member_offset(struct rw007_cmd, params) + wifi_cmd->len;

    rt_mb_send(&wifi_device->spi_tx_mb, (rt_uint32_t)data_packet);
    rt_event_send(&spi_wifi_data_event, 1);

    result = rt_mb_recv(&wifi_device->rw007_cmd_mb,
                        (rt_ubase_t *)&resp,
                        timeout);

    if (result != RT_EOK)
    {
        LOG_E("CMD %d error, result %d", cmd, result);
    }

    if (resp != RT_NULL)
        result = resp->result;

_exit:
    wifi_device->last_cmd = 0;
    if (resp)
    {
        rt_free(resp);
    }
    return result;
}

static rt_err_t spi_wifi_transfer(struct rw007_wifi *dev)
{
    struct pbuf *p = RT_NULL;
    struct spi_cmd_request cmd;
    struct spi_response resp;

    rt_err_t result;
    const struct spi_data_packet *data_packet = RT_NULL;

    struct rw007_wifi *wifi_device = (struct rw007_wifi *)dev;
    struct rt_spi_device *rt_spi_device = wifi_device->rt_spi_device;

    /* Disable INT Pin interrupt */
    spi_wifi_int_cmd(0);

    while (spi_wifi_is_busy())
    {
        /* wait for idel */
    }

    /* Clear cmd */
    memset(&cmd, 0, sizeof(struct spi_cmd_request));

    /* Set magic word */
    cmd.magic1 = CMD_MAGIC1;
    cmd.magic2 = CMD_MAGIC2;

    /* Set master ready flag bit */
    cmd.flag |= CMD_FLAG_MRDY;

    /* Try get data to send to rw007 */
    result = rt_mb_recv(&wifi_device->spi_tx_mb,
                        (rt_ubase_t *)&data_packet,
                        0);
    /* Set length for master to slave when data ready*/
    if ((result == RT_EOK) && (data_packet != RT_NULL) && (data_packet->data_len > 0))
    {
        cmd.M2S_len = data_packet->data_len + member_offset(struct spi_data_packet, buffer);
    }

    /* Stage 1: Send command to rw007 */
    rt_spi_send(rt_spi_device, &cmd, sizeof(cmd));
    while (spi_wifi_is_busy())
    {
        /* wait for idel */
    }

    /* Stage 2: Receive response from rw007 and transmit data */
    {
        struct rt_spi_message message;
        uint32_t max_data_len = 0;

        /* Setup message */
        message.send_buf = RT_NULL;
        message.recv_buf = &resp;
        message.length = sizeof(resp);
        message.cs_take = 1;
        message.cs_release = 0;

        /* Start a SPI transmit */
        rt_spi_take_bus(rt_spi_device);

        /* Receive response from rw007 */
        rt_spi_device->bus->ops->xfer(rt_spi_device, &message);

        /* Check response's magic word */
        if ((resp.magic1 != RESP_MAGIC1) || (resp.magic2 != RESP_MAGIC2))
        {
            goto _bad_resp_magic;
        }

        /* Check rw007's data ready flag */
        if (resp.flag & RESP_FLAG_SRDY)
        {
            max_data_len = cmd.M2S_len;
        }

        if (resp.S2M_len)
        {
            if (resp.S2M_len > MAX_SPI_PACKET_SIZE)
            {
                /* Drop error data */
                resp.S2M_len = 0;
            }

            if (resp.S2M_len > max_data_len)
                max_data_len = resp.S2M_len;
        }

    _bad_resp_magic:
        /* Setup message */
        message.send_buf = data_packet;
        message.recv_buf = wifi_device->spi_hw_rx_buffer;
        message.length = RT_ALIGN(max_data_len, 4);/* align clk to word */
        message.cs_take = 0;
        message.cs_release = 1;

        /* Transmit data */
        rt_spi_device->bus->ops->xfer(rt_spi_device, &message);

        /* End a SPI transmit */
        rt_spi_release_bus(rt_spi_device);

        /* Free send data space */
        if (cmd.M2S_len)
        {
            rt_mp_free((void *)data_packet);
            data_packet = RT_NULL;
        }

        /* Parse recevied data */
        if ((resp.S2M_len) && (resp.S2M_len <= MAX_SPI_PACKET_SIZE))
        {
            data_packet = (struct spi_data_packet *)wifi_device->spi_hw_rx_buffer;
            if (data_packet->data_type == data_type_eth_data)
            {

                if (wifi_device->active)
                {
                    p = pbuf_alloc(PBUF_LINK, data_packet->data_len, PBUF_RAM);
                    pbuf_take(p, (rt_uint8_t *)data_packet->buffer, data_packet->data_len);

                    rt_mb_send(&wifi_device->eth_rx_mb, (rt_uint32_t)p);
                    eth_device_ready((struct eth_device *)dev);
                }
            }
            else if (data_packet->data_type == data_type_resp)
            {
                resp_handler(dev, (struct rw007_resp *)data_packet->buffer);
            }
        }
    }
    /* Enable INT Pin interrupt */
    spi_wifi_int_cmd(1);

    if ((cmd.M2S_len == 0) && (resp.S2M_len == 0))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

/********************************* RT-Thread Ethernet interface begin **************************************/
static rt_err_t rw007_wifi_init(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t rw007_wifi_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t rw007_wifi_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_size_t rw007_wifi_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_set_errno(-RT_ENOSYS);
    return 0;
}

static rt_size_t rw007_wifi_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_set_errno(-RT_ENOSYS);
    return 0;
}

static rt_err_t rw007_wifi_control(rt_device_t dev, int cmd, void *args)
{
    struct rw007_wifi *wifi_device = (struct rw007_wifi *)dev;
    rt_err_t result = RT_EOK;

    if (cmd == NIOCTL_GADDR)
    {
        memcpy(args, wifi_device->dev_addr, 6);
    }
    else
    {
        result = rw007_cmd(wifi_device, cmd, args);
    }

    return result;
}

/* transmit packet. */
rt_err_t rw007_wifi_tx(rt_device_t dev, struct pbuf *p)
{
    rt_err_t result = RT_EOK;
    struct spi_data_packet *data_packet;
    struct rw007_wifi *wifi_device = (struct rw007_wifi *)dev;

    if (!wifi_device->active)
    {
        return RT_EOK;
    }

    /* get free tx buffer */
    data_packet = (struct spi_data_packet *)rt_mp_alloc(&wifi_device->spi_tx_mp, RT_WAITING_FOREVER);
    if (data_packet != RT_NULL)
    {
        data_packet->data_type = data_type_eth_data;
        data_packet->data_len = p->tot_len;

        pbuf_copy_partial(p, data_packet->buffer, data_packet->data_len, 0);

        rt_mb_send(&wifi_device->spi_tx_mb, (rt_uint32_t)data_packet);
        rt_event_send(&spi_wifi_data_event, 1);
    }
    else
        return -RT_ERROR;

    /* Return SUCCESS */
    return result;
}

/* reception packet. */
struct pbuf *rw007_wifi_rx(rt_device_t dev)
{
    struct pbuf *p = RT_NULL;
    struct rw007_wifi *wifi_device = (struct rw007_wifi *)dev;

    if (rt_mb_recv(&wifi_device->eth_rx_mb, (rt_ubase_t *)&p, 0) != RT_EOK)
    {
        return RT_NULL;
    }

    return p;
}
/********************************* RT-Thread Ethernet interface end **************************************/

static void spi_wifi_data_thread_entry(void *parameter)
{
    rt_uint32_t e;
    rt_err_t result;

    while (1)
    {
        /* receive first event */
        if (rt_event_recv(&spi_wifi_data_event,
                          1,
                          RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                          RT_WAITING_FOREVER,
                          &e) != RT_EOK)
        {
            continue;
        }

        result = spi_wifi_transfer(&rw007_wifi_device);

        if (result == RT_EOK)
        {
            rt_event_send(&spi_wifi_data_event, 1);
        }
    }
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops rw007_ops =
{
    rw007_wifi_init,
    rw007_wifi_open,
    rw007_wifi_close,
    rw007_wifi_read,
    rw007_wifi_write,
    rw007_wifi_control
};
#endif

rt_err_t rt_hw_wifi_init(const char *spi_device_name, wifi_mode_t mode)
{
    /* align and struct size check. */
    RT_ASSERT((SPI_MAX_DATA_LEN & 0x03) == 0);
    RT_ASSERT(sizeof(struct rw007_resp) <= SPI_MAX_DATA_LEN);

    memset(&rw007_wifi_device, 0, sizeof(struct rw007_wifi));

    rw007_wifi_device.rt_spi_device = (struct rt_spi_device *)rt_device_find(spi_device_name);

    if (rw007_wifi_device.rt_spi_device == RT_NULL)
    {
        LOG_E("spi device %s not found!\r", spi_device_name);
        return -RT_ENOSYS;
    }

    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MODE_0 | RT_SPI_MSB; /* SPI Compatible: Mode 0. */
        cfg.max_hz = 30 * 1000 * 1000;             /* max 30M */
        rt_spi_configure(rw007_wifi_device.rt_spi_device, &cfg);
    }

#ifdef RT_USING_DEVICE_OPS
    rw007_wifi_device.parent.parent.ops = &rw007_ops;
#else
    rw007_wifi_device.parent.parent.init = rw007_wifi_init;
    rw007_wifi_device.parent.parent.open = rw007_wifi_open;
    rw007_wifi_device.parent.parent.close = rw007_wifi_close;
    rw007_wifi_device.parent.parent.read = rw007_wifi_read;
    rw007_wifi_device.parent.parent.write = rw007_wifi_write;
    rw007_wifi_device.parent.parent.control = rw007_wifi_control;
#endif
    rw007_wifi_device.parent.parent.user_data = RT_NULL;

    rw007_wifi_device.parent.eth_rx = rw007_wifi_rx;
    rw007_wifi_device.parent.eth_tx = rw007_wifi_tx;

    rt_mp_init(&rw007_wifi_device.spi_tx_mp,
               "spi_tx",
               &rw007_wifi_device.spi_tx_mempool[0],
               sizeof(rw007_wifi_device.spi_tx_mempool),
               sizeof(struct spi_data_packet));

    rt_mp_init(&rw007_wifi_device.spi_rx_mp,
               "spi_rx",
               &rw007_wifi_device.spi_rx_mempool[0],
               sizeof(rw007_wifi_device.spi_rx_mempool),
               sizeof(struct spi_data_packet));

    rt_mb_init(&rw007_wifi_device.spi_tx_mb,
               "spi_tx",
               &rw007_wifi_device.spi_tx_mb_pool[0],
               SPI_TX_POOL_SIZE,
               RT_IPC_FLAG_PRIO);

    rt_mb_init(&rw007_wifi_device.eth_rx_mb,
               "eth_rx",
               &rw007_wifi_device.eth_rx_mb_pool[0],
               SPI_TX_POOL_SIZE,
               RT_IPC_FLAG_PRIO);

    rt_mb_init(&rw007_wifi_device.rw007_cmd_mb,
               "wifi_cmd",
               &rw007_wifi_device.rw007_cmd_mb_pool[0],
               sizeof(rw007_wifi_device.rw007_cmd_mb_pool) / 4,
               RT_IPC_FLAG_PRIO);
    rt_event_init(&spi_wifi_data_event, "wifi", RT_IPC_FLAG_FIFO);

    spi_wifi_hw_init();

    {
        rt_thread_t tid;

        tid = rt_thread_create("wifi",
                               spi_wifi_data_thread_entry,
                               RT_NULL,
                               2048,
                               RT_THREAD_PRIORITY_MAX - 2,
                               20);

        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }

    /* init: get mac address */
    {
        rw007_cmd_init init;
        init.mode = mode;
        LOG_D("wifi_control init");
        rw007_wifi_control((rt_device_t)&rw007_wifi_device,
                           RW007_CMD_INIT,
                           (void *)&init); // 0: firmware, 1: STA, 2:AP
    }

    /* register eth device */
    eth_device_init(&(rw007_wifi_device.parent), "w0");
    eth_device_linkchange(&rw007_wifi_device.parent, RT_FALSE);

    return RT_EOK;
}

void spi_wifi_isr(int vector)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_event_send(&spi_wifi_data_event, 1);

    /* leave interrupt */
    rt_interrupt_leave();
}

/********************************* RW007 tools **************************************/
rt_err_t rw007_join(const char *SSID, const char *passwd)
{
    rt_err_t result;
    rt_device_t wifi_device;
    rw007_cmd_easy_join easy_join;

    wifi_device = rt_device_find("w0");
    if (wifi_device == RT_NULL)
        return -RT_ENOSYS;

    strncpy(easy_join.ssid, SSID, sizeof(easy_join.ssid));
    strncpy(easy_join.passwd, passwd, sizeof(easy_join.passwd));

    result = rt_device_control(wifi_device,
                               RW007_CMD_EASY_JOIN,
                               (void *)&easy_join);

    return result;
}

rt_err_t rw007_softap(const char *SSID, const char *passwd, uint32_t security, uint32_t channel)
{
    rt_err_t result;
    rt_device_t wifi_device;
    rw007_cmd_softap softap;

    wifi_device = rt_device_find("w0");
    if (wifi_device == RT_NULL)
        return -RT_ENOSYS;

    strncpy(softap.ssid, SSID, sizeof(softap.ssid));
    strncpy(softap.passwd, passwd, sizeof(softap.passwd));

    softap.security = security;
    softap.channel = channel;
    result = rt_device_control(wifi_device,
                               RW007_CMD_SOFTAP,
                               (void *)&softap);

    return result;
}

int32_t rw007_rssi(void)
{
    rt_err_t result;
    struct rw007_wifi *wifi_device;
    rw007_cmd_rssi rssi_cmd;

    wifi_device = (struct rw007_wifi *)rt_device_find("w0");

    if (wifi_device == RT_NULL)
        return 0;

    if (wifi_device->active == 0)
        return 0;

    rt_memcpy(rssi_cmd.bssid, wifi_device->dev_addr, 6);

    // SCAN
    result = rt_device_control((rt_device_t)wifi_device,
                               RW007_CMD_RSSI,
                               &rssi_cmd);

    if (result == RT_EOK)
    {
        rt_kprintf("rssi: %d\n", wifi_device->ap_info.rssi);
        return wifi_device->ap_info.rssi;
    }
    else
    {
        rt_kprintf("rssi: error\n");
    }

    return (-1);
}

#ifdef RT_USING_FINSH
#include <finsh.h>

static rt_err_t rw007_scan(void)
{
    rt_err_t result;
    struct rw007_wifi *wifi_device;

    wifi_device = (struct rw007_wifi *)rt_device_find("w0");

    rt_kprintf("being scan \n");
    result = rt_device_control((rt_device_t)wifi_device,
                               RW007_CMD_SCAN,
                               RT_NULL);

    rt_kprintf("scan result:%d", result);

    if (result == RT_EOK)
    {
        uint32_t i;
        rw007_ap_info *ap_info;

        for (i = 0; i < wifi_device->ap_scan_count; i++)
        {
            ap_info = &wifi_device->ap_scan[i];
            rt_kprintf("AP #%02d SSID: %-32.32s\n", i, ap_info->ssid);
        }
    }

    return result;
}

static int wifi_join(int argc, char *args[])
{
    if (argc != 3)
        return -1;
    return rw007_join(args[1], args[2]);
}

FINSH_FUNCTION_EXPORT(rw007_scan, SACN and list AP.);
FINSH_FUNCTION_EXPORT(rw007_join, RW007 join to AP.);
FINSH_FUNCTION_EXPORT(rw007_rssi, get RW007 current AP rssi.);

MSH_CMD_EXPORT(wifi_join, wifi_join);
MSH_CMD_EXPORT(rw007_rssi, rw007_rssi);
MSH_CMD_EXPORT(rw007_scan, rw007_scan);
#endif
