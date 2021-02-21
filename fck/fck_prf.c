#include "../af_common.h"
#include "../af_config.h"
#include "fck_ie80211.h"

Mac_Addr victim_ap_list[AF_FCK_AP_COUNT];
uint8_t victim_ap_count = 0;

void fck_prf_collect_ssid()
{
    ieee80211_header *pkt = NULL;

    FRAME_TYPE  fm_type;
    DS_TYPE     ds_type;

    // clear ap list
    victim_ap_count = 0;

    while ()
    pkt = psp_wifi_read_pkt();

    if (pkt)
    {
        fm_type = COPY_UINT16(pkt->ctl_frame) & MASK_FM;
        ds_type = SWAP_UINT16(pkt->ctl_frame) & MASK_DS;

        if (fm_type == FT_MGT_PROBE_REQ)
        {

            // add to list
        }
    }
}

void fck_prf()
{
    uint8_t channel;

    // listen for a while
    fck_collect_ssid()

    // attack, regardless response

    channel = psp_wifi_hop();
    af_log("[%d] ", channel);

    while (1)
    {
        /*

        if (pkt)
        {

        }
        */

        psp_wifi_write_pkt((ieee80211_header *)ie80211_beacon, sizeof(ie80211_beacon));
    }

    psp_sleep(AF_CHANNEL_HOP_INTVL);
}
