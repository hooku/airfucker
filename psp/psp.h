#ifndef __PSP_H_
#define __PSP_H_

/* Common interface includes */
#include "common.h"
#include "simplelink.h"
#include "wlan.h"
#include "uart_if.h"

#define DEVICE_NOT_IN_STATION_MODE -1

#define psp_printf  UART_PRINT

typedef enum
{
    PSP_POWER_LED,
    PSP_SYS_LED,
    PSP_RF_LED,
    PSP_EasyLink_BUTTON,
    PSP_WakeUp_BUTTON,
    PSP_GPIO_PINS_MAX,
} psp_gpio_pins_t;

typedef enum
{
    PSP_GPIO_UNUSED,
    PSP_GPIO_0,
    PSP_GPIO_1,
    PSP_GPIO_2,
    PSP_GPIO_MAX,
} psp_gpio_t;

typedef struct
{
    unsigned long gpio_base;
    unsigned long gpio_pin;
    unsigned long gpio_pin_mode;
} platform_pin_mapping_t;

/* platform */
extern int8_t psp_init_platform();
extern void psp_sleep(uint32_t ms);

/* gpio */
extern int8_t psp_gpio_init(psp_gpio_t gpio);
extern int8_t psp_gpio_set(psp_gpio_t gpio, uint8_t val);

/* wifi */
extern int16_t psp_wifi_default_state();
extern int16_t psp_wifi_monitor_mode();
extern uint8_t psp_wifi_hop(void);
extern ieee80211_header *psp_wifi_read_pkt();
extern int16_t psp_wifi_write_pkt(ieee80211_header *pkt, short len);

#endif //  __PSP_H_
