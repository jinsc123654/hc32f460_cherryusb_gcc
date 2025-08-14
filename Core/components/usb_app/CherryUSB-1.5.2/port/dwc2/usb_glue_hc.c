/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usb_config.h"
#include "usb_dwc2_reg.h"
#include "usb_dwc2_param.h"
#include "string.h"

/* When using [GPIO_SetFunc(USBF_VBUS_PORT, USBF_VBUS_PIN, USBF_VBUS_FUNC);],
 * there is no need to configure GOTGCTL
 */

extern uint32_t SystemCoreClock;

/**
 * @brief Delay function in milliseconds
 * @param ms: number of milliseconds to delay
 */
void usbd_dwc2_delay_ms(uint8_t ms)
{
    uint32_t count = SystemCoreClock / 1000 * ms;
    while (count--) {
        __asm volatile("nop");
    }
}


/* HC32F460 USB parameters with DMA enabled */
/* USB configuration for HC32F460, using DMA and safe FIFO allocation */
static const struct dwc2_user_params hc32f460_param = {
    .phy_type               = DWC2_PHY_TYPE_PARAM_FS,   // Full-Speed PHY
    .device_dma_enable      = true,                     // Enable DMA
    .device_dma_desc_enable = false,                    // Descriptor DMA disabled
    .total_fifo_size        = 320,                      // Total FIFO size (in words)
    .device_rx_fifo_size    = 320 - (16 * 7),           // RX FIFO size (in words)
    .device_tx_fifo_size = {                             // TX FIFO size for each endpoint (in words)
        [0]  = 16, // EP0 control endpoint 64 bytes
        [1]  = 16, // EP1
        [2]  = 16, // EP2
        [3]  = 16, // EP3
        [4]  = 16, // EP4
        [5]  = 16, // EP5
        [6]  = 16, // EP6
        [7]  = 0,
        [8]  = 0,
        [9]  = 0,
        [10] = 0,
        [11] = 0,
        [12] = 0,
        [13] = 0,
        [14] = 0,
        [15] = 0
    },
    .b_session_valid_override = true,                   // Force VBUS valid
};


#ifndef CONFIG_USB_DWC2_CUSTOM_PARAM
/**
 * @brief Retrieve default DWC2 user parameters
 * @param reg_base: base address of USB registers (not used)
 * @param params: pointer to store retrieved parameters
 */
void dwc2_get_user_params(uint32_t reg_base, struct dwc2_user_params *params)
{
    (void)reg_base;

    // Copy default parameters
    memcpy(params, &hc32f460_param, sizeof(struct dwc2_user_params));

#ifdef CONFIG_USB_DWC2_CUSTOM_FIFO
    // If user-defined FIFO configuration is enabled
    struct usb_dwc2_user_fifo_config fifo_cfg;
    dwc2_get_user_fifo_config(reg_base, &fifo_cfg);

    params->device_rx_fifo_size = fifo_cfg.device_rx_fifo_size;
    for (uint8_t i = 0; i < MAX_EPS_CHANNELS; i++) {
        params->device_tx_fifo_size[i] = fifo_cfg.device_tx_fifo_size[i];
    }
#endif
}
#endif
