#include "main.h"
#include "string.h"

#define LL_PERIPH_SEL (LL_PERIPH_GPIO | LL_PERIPH_FCG | LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_EFM | LL_PERIPH_SRAM)

int usb_bsp_init(void);
void cdc_acm_init(uint8_t busid, uintptr_t reg_base);
void cdc_acm_data_send_with_dtr_test(uint8_t busid);
int main(void)
{
    SEGGER_RTT_Init();
    print_rtt("系统启动 使用外部8Mhz晶振\r\n");
    LL_PERIPH_WE(LL_PERIPH_SEL);
    BSP_CLK_Init();
    LL_PERIPH_WP(LL_PERIPH_SEL);

#ifdef USE_CHERRY_USB_1_3_0
    usb_bsp_init();
#endif
    cdc_acm_init(0, CM_USBFS_BASE);
    while (1)
    {
        cdc_acm_data_send_with_dtr_test(0);
    }
    return 0;
}