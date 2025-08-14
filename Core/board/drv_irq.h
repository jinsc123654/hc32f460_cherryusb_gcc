/*
 * Copyright (C) 2022-2024, Xiaohua Semiconductor Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-28     CDT          first version
 */

#ifndef __DRV_IRQ_H__
#define __DRV_IRQ_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll.h"
#include "stdint.h"
#include "stdbool.h"
#include "irq_config.h"
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
struct hc32_irq_config
{
    IRQn_Type       irq_num;
    uint32_t        irq_prio;
    en_int_src_t    int_src;
};

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
 int hc32_install_irq_handler(struct hc32_irq_config *irq_config,
                              void (*irq_hdr)(void),
                              bool irq_enable);
#if defined (HC32F448)
 int hc32_install_independ_irq_handler(struct hc32_irq_config *irq_config,
                                       bool irq_enable);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __DRV_IRQ_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
