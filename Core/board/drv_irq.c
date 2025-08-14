/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "drv_irq.h"

/*******************************************************************************
 * Function implementation
 ******************************************************************************/

/**
 * @brief 安装外设中断处理函数
 * @param irq_config 中断配置指针
 * @param irq_hdr 中断回调函数指针
 * @param irq_enable 是否使能中断
 * @return 0 成功, -1 失败
 */
int hc32_install_irq_handler(struct hc32_irq_config *irq_config,
                             void (*irq_hdr)(void),
                             bool irq_enable)
{
    if (irq_config == NULL || irq_hdr == NULL)
        return -1;

    stc_irq_signin_config_t stcIrqSignConfig;
    stcIrqSignConfig.enIRQn      = irq_config->irq_num;
    stcIrqSignConfig.enIntSrc    = irq_config->int_src;
    stcIrqSignConfig.pfnCallback = irq_hdr;

    if (LL_OK == INTC_IrqSignIn(&stcIrqSignConfig))
    {
        NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
        NVIC_SetPriority(stcIrqSignConfig.enIRQn, irq_config->irq_prio);

        if (irq_enable)
            NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
        else
            NVIC_DisableIRQ(stcIrqSignConfig.enIRQn);

        return 0;
    }

    return -1;
}

#if defined (HC32F448)
/**
 * @brief 安装独立中断处理函数（仅 HC32F448）
 * @param irq_config 中断配置指针
 * @param irq_enable 是否使能中断
 * @return 0 成功, -1 失败
 */
int hc32_install_independ_irq_handler(struct hc32_irq_config *irq_config,
                                      bool irq_enable)
{
    if (irq_config == NULL)
        return -1;

    NVIC_ClearPendingIRQ(irq_config->irq_num);
    NVIC_SetPriority(irq_config->irq_num, irq_config->irq_prio);

    if (irq_enable)
    {
        INTC_IntSrcCmd(irq_config->int_src, ENABLE);
        NVIC_EnableIRQ(irq_config->irq_num);
    }
    else
    {
        INTC_IntSrcCmd(irq_config->int_src, DISABLE);
        NVIC_DisableIRQ(irq_config->irq_num);
    }

    return 0;
}
#endif
