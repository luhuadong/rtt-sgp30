/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-23     luhuadong    the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdlib.h>
#include "sgp30.h"

/*!
 *  @brief  Get TVOC value from SGP30 sensor
 */
static void sgp30_read_tvoc(void)
{
    rt_device_t tvoc_dev = RT_NULL;
    struct rt_sensor_data sensor_data;

    tvoc_dev = rt_device_find("tvoc_sg3");
    if (!tvoc_dev)
    {
        rt_kprintf("Can't find TVOC device.\n");
        return;
    }

    if (rt_device_open(tvoc_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open TVOC device failed.\n");
        return;
    }

    if (1 != rt_device_read(tvoc_dev, 0, &sensor_data, 1)) 
    {
        rt_kprintf("Read TVOC data failed.\n");
    }
    rt_kprintf("[%d] TVOC: %d ppb\n", sensor_data.timestamp, sensor_data.data.tvoc);

    rt_device_close(tvoc_dev);
}

/*!
 *  @brief  Get eCO2 value from SGP30 sensor
 */
static void sgp30_read_eco2(void)
{
    rt_device_t eco2_dev = RT_NULL;
    struct rt_sensor_data sensor_data;

    eco2_dev = rt_device_find("eco2_sg3");
    if (!eco2_dev) 
    {
        rt_kprintf("Can't find eCO2 device.\n");
        return;
    }

    if (rt_device_open(eco2_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open eCO2 device failed.\n");
        return;
    }

    if (1 != rt_device_read(eco2_dev, 0, &sensor_data, 1)) 
    {
        rt_kprintf("Read eCO2 data failed.\n");
    }
    rt_kprintf("[%d] eCO2: %d ppm\n", sensor_data.timestamp, sensor_data.data.eco2);

    rt_device_close(eco2_dev);
}

/*!
 *  @brief  Get Baseline value from SGP30 sensor
 */
static void sgp30_read_baseline(void)
{
    rt_device_t sgp30_dev = RT_NULL;
    struct sgp30_baseline baseline;

    sgp30_dev = rt_device_find("tvoc_sg3");
    if (!sgp30_dev) 
    {
        rt_kprintf("Can't find SGP30 device.\n");
        return;
    }

    if (rt_device_open(sgp30_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open SGP30 device failed.\n");
        return;
    }

    if (RT_EOK != rt_device_control(sgp30_dev, RT_SENSOR_CTRL_GET_BASELINE, &baseline)) 
    {
        rt_kprintf("Get baseline failed.\n");
    }
    rt_kprintf("eCO2 baseline: %d, TVOC baseline: %d\n", baseline.eco2_base, baseline.tvoc_base);

    rt_device_close(sgp30_dev);
}

/*!
 *  @brief  Set Baseline value from SGP30 sensor
 */
static void sgp30_calib_baseline(int argc, char **argv)
{
    rt_device_t sgp30_dev = RT_NULL;
    struct sgp30_baseline baseline;
    int  result = 0;
    char answer = 0;

    sgp30_dev = rt_device_find("tvoc_sg3");
    if (!sgp30_dev) 
    {
        rt_kprintf("Can't find SGP30 device.\n");
        return;
    }

    if (rt_device_open(sgp30_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open SGP30 device failed.\n");
        return;
    }

    /* get baseline */
    if (argc == 3)
    {
        baseline.eco2_base = atoi(argv[1]);
        baseline.tvoc_base = atoi(argv[2]);
    }
    else
    {
        result = rt_device_control(sgp30_dev, RT_SENSOR_CTRL_GET_BASELINE, &baseline);
    }

    if (result != RT_EOK)
    {
        rt_kprintf("Invalid baseline.\n");
        rt_device_close(sgp30_dev);
        return;
    }
    rt_kprintf("Setting baseline (eCO2: %d, TVOC: %d)...\n", baseline.eco2_base, baseline.tvoc_base);

    /* set baseline */
    if (RT_EOK != rt_device_control(sgp30_dev, RT_SENSOR_CTRL_SET_BASELINE, &baseline)) 
    {
        rt_kprintf("Set baseline failed.\n");
    }
    rt_kprintf("Set baseline finished.\n");

    rt_device_close(sgp30_dev);
}

/*!
 *  @brief  Set humidity to SGP30 sensor
 */
static void sgp30_set_humi(int argc, char **argv)
{
    rt_device_t sgp30_dev = RT_NULL;
    rt_uint32_t absolute_humidity;

    if (argc != 2)
    {
        rt_kprintf("Invalid input. Usage: sgp30_set_humidity <absolute_humidity>\n");
        return;
    }

    absolute_humidity = atoi(argv[1]);

    sgp30_dev = rt_device_find("tvoc_sg3");
    if (!sgp30_dev) 
    {
        rt_kprintf("Can't find SGP30 device.\n");
        return;
    }

    if (rt_device_open(sgp30_dev, RT_DEVICE_FLAG_RDWR)) 
    {
        rt_kprintf("Open SGP30 device failed.\n");
        return;
    }

    if (RT_EOK != rt_device_control(sgp30_dev, RT_SENSOR_CTRL_SET_HUMIDITY, (void *)absolute_humidity)) 
    {
        rt_kprintf("Set humidity failed.\n");
    }
    rt_kprintf("Set humidity %d finished.\n", absolute_humidity);

    rt_device_close(sgp30_dev);
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(sgp30_read_tvoc, Get sgp30 TVOC data);
MSH_CMD_EXPORT(sgp30_read_eco2, Get sgp30 eCO2 data);
MSH_CMD_EXPORT(sgp30_read_baseline, Get sgp30 TVOC and eCO2 baseline);
MSH_CMD_EXPORT(sgp30_calib_baseline, Set sgp30 TVOC and eCO2 baseline. Usage: sgp30_set_baseline [eco2_base] [tvoc_base]);
MSH_CMD_EXPORT(sgp30_set_humi, Set humidity to sgp30. Usage: sgp30_set_humidity <absolute_humidity>);
#endif
