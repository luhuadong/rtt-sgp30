/*
 * Copyright (c) 2020, RudyLo <luhuadong@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-22     luhuadong    the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "sgp30.h"

#ifdef PKG_USING_SGP30_SAMPLE_I2C_BUS_NAME
#define SGP30_I2C_BUS_NAME       PKG_USING_SGP30_SAMPLE_I2C_BUS_NAME
#else
#define SGP30_I2C_BUS_NAME       "i2c1"
#endif

#define TEST_COUNT               20

/* cat_sgp30 */
static void cat_sgp30(void)
{
    sgp30_device_t sgp30 = sgp30_create(SGP30_I2C_BUS_NAME);

    if(!sgp30) 
    {
        rt_kprintf("(SGP30) Init failed\n");
        return;
    }

    for (int i=0; i<TEST_COUNT; i++)
    {
        /* Read TVOC and eCO2 */
        if(!sgp30_measure(sgp30)) 
        {
            rt_kprintf("(SGP30) Measurement failed\n");
            sgp30_delete(sgp30);
            break;
        }

        /* Read rawH2 and rawEthanol */
        if(!sgp30_measure_raw(sgp30)) 
        {
            rt_kprintf("(SGP30) Raw Measurement failed\n");
            sgp30_delete(sgp30);
            break;
        }

        rt_kprintf("[%2u] TVOC: %d ppb, eCO2: %d ppm; Raw H2: %d, Raw Ethanol: %d\n", 
                   i, sgp30->TVOC, sgp30->eCO2, sgp30->rawH2, sgp30->rawEthanol);

        rt_thread_mdelay(1500);
    }
    
    sgp30_delete(sgp30);
}
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(cat_sgp30, read sgp30 TVOC and eCO2);
#endif
