#ifndef CHERRYUSB_CONFIG_H
#define CHERRYUSB_CONFIG_H

#if USE_CHERRY_USB_1_3_0
#include "./include/usb_config_1.3.0.h"
#endif

#ifdef USE_CHERRY_USB_1_5_2
#include "./include/usb_config_1.5.2.h"
#endif

#endif /* CHERRYUSB_CONFIG_H */
