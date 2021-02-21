#include "af_common.h"
#include "af_config.h"

extern uint8_t ie80211_beacon[158];

void af_init(void)
{
    /* init platform specific */
    psp_init_platform();

    /* init wifi */
    af_log("set wifi to default state..");
    psp_wifi_default_state();
    af_log("set wifi to monitor mode..");
    psp_wifi_monitor_mode();  
}


int main(void)
{
    af_init();
    af_fuck();

    while(1)
      ;
}
