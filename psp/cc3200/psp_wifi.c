#include "../../af_common.h"
#include "../../af_config.h"
#include "../psp.h"
#include "../ie80211/ie80211.h"

unsigned long   g_ulStatus      = 0;            // global link(layer2 & layer3) status
_i16            g_80211_sock    = 0;
int             g_channel       = 6;

_i8 g_pkt_buffer[AF_80211_MAX_PKT];

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->EventData.deviceEvent.status,
               pDevEvent->EventData.deviceEvent.sender);
}

void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
}

int16_t psp_wifi_default_state()
{
    SlVersionFull ver = {0};
    _WlanRxFilterOperationCommandBuff_t RxFilterIdMask = {0};

    unsigned char ucVal = 1;
    unsigned char ucConfigOpt = 0;
    unsigned char ucConfigLen = 0;
    unsigned char ucPower = 0;

    int16_t ret = FAILURE;
    long lMode = -1;

    lMode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(lMode);

    // If the device is not in station-mode, try configuring it in station-mode 
    if (ROLE_STA != lMode)
    {
        // Switch to STA role and restart 
        ret = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(ret);

        ret = sl_Stop(0xFF);
        ASSERT_ON_ERROR(ret);

        ret = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(ret);

        // Check if the device is in station again 
        if (ROLE_STA != ret)
        {
            // We don't want to proceed if the device is not coming up in STA-mode 
            return DEVICE_NOT_IN_STATION_MODE;
        }
    }
    
    // Get the device's version-information
    ucConfigOpt = SL_DEVICE_GENERAL_VERSION;
    ucConfigLen = sizeof(ver);
    ret = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt, 
                                &ucConfigLen, (unsigned char *)(&ver));
    ASSERT_ON_ERROR(ret);
    
    UART_PRINT("Host Driver Version: %s\n\r",SL_DRIVER_VERSION);
    UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r",
    ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
    ver.ChipFwAndPhyVersion.FwVersion[0],ver.ChipFwAndPhyVersion.FwVersion[1],
    ver.ChipFwAndPhyVersion.FwVersion[2],ver.ChipFwAndPhyVersion.FwVersion[3],
    ver.ChipFwAndPhyVersion.PhyVersion[0],ver.ChipFwAndPhyVersion.PhyVersion[1],
    ver.ChipFwAndPhyVersion.PhyVersion[2],ver.ChipFwAndPhyVersion.PhyVersion[3]);

    // Set connection policy to Auto + SmartConfig 
    //      (Device's default connection policy)
    ret = sl_WlanPolicySet(SL_POLICY_CONNECTION, 
                                SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(ret);

    // Remove all profiles
    ret = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(ret);

    //
    // Device in station-mode. Disconnect previous connection if any
    // The function returns 0 if 'Disconnected done', negative number if already
    // disconnected Wait for 'disconnection' event if 0 is returned, Ignore 
    // other return-codes
    //
    ret = sl_WlanDisconnect();
    if(0 == ret)
    {
        // Wait
        while(IS_CONNECTED(g_ulStatus))
        {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask(); 
#endif
        }
    }

    // Disable DHCP client
    ret = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&ucVal);
    ASSERT_ON_ERROR(ret);

    // Disable scan
    ucConfigOpt = SL_SCAN_POLICY(0);
    ret = sl_WlanPolicySet(SL_POLICY_SCAN , ucConfigOpt, NULL, 0);
    ASSERT_ON_ERROR(ret);

    // Set Tx power level for station mode
    // Number between 0-15, as dB offset from max power - 0 will set max power
    ucPower = 0;
    ret = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, 
            WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *)&ucPower);
    ASSERT_ON_ERROR(ret);

    // Set PM policy to normal
    ret = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(ret);

    // Unregister mDNS services
    ret = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(ret);

    // Remove all 64 filters (8*8)
    memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    ret = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(ret);

    return ret;
}

int16_t psp_wifi_monitor_mode()
{
    int16_t ret = FAILURE;

    ret = sl_Stop(0xFF);
    ret = sl_Start(0, 0, 0);

    return ret;
}

uint8_t psp_wifi_hop()
{
    struct SlTimeval_t  time_value;

    static _i8 i_ch;

    i_ch = (i_ch + 1)%AF_CHANNEL_MAX;
    g_channel = ch_hoop[i_ch];
    
    if (g_80211_sock)
    {
        sl_Close(g_80211_sock);
    }
    g_80211_sock = sl_Socket(SL_AF_RF, SL_SOCK_RAW, ch_hoop[i_ch]);

    time_value.tv_sec   = 0;
    time_value.tv_usec  = 1000;
    sl_SetSockOpt(g_80211_sock, SL_SOL_SOCKET, SL_SO_RCVTIMEO, (_u8 *)&time_value, sizeof(time_value));

    return g_channel;
}

ieee80211_header *psp_wifi_read_pkt()
{
    _i16 wlan_result;
    
    static uint16_t     last_seq;
    
    SlTransceiverRxOverHead_t   *hdr_rx_overhead    = (SlTransceiverRxOverHead_t *)g_pkt_buffer;
    ieee80211_header            *hdr_80211          = (ieee80211_header *)(g_pkt_buffer + sizeof(SlTransceiverRxOverHead_t));
    
    wlan_result = sl_Recv(g_80211_sock, g_pkt_buffer, AF_80211_MAX_PKT, 0);
    
    if ((wlan_result <= 0) ||
        (wlan_result <= (sizeof(SlTransceiverRxOverHead_t) + sizeof(ieee80211_header))))
    {
        return 0;
    }

    // reject duplicated pkt:
    if (hdr_80211->ctl_seq == last_seq)
    {
        return 0;
    }
    last_seq = hdr_80211->ctl_seq;          // filter duplicate len value (TODO: we need figure out why)
    
    return hdr_80211;
}

int16_t psp_wifi_write_pkt(ieee80211_header *pkt, short len)
{
    int16_t ret = FAILURE;

    _i16 wlan_result;

    wlan_result = sl_Send(g_80211_sock, pkt, len, SL_RAW_RF_TX_PARAMS(g_channel,  RATE_11M, 0, AF_80211_PREAMBLE));

    if(wlan_result < 0)
    {
        sl_Close(g_80211_sock);
        ASSERT_ON_ERROR(wlan_result);
    }

    return ret;
}

_u8 psp_wifi_scan(Sl_WlanNetworkEntry_t *sl_ap_list)
{
    _u8 policy;
    _u32 interval_sec;
    
    _u8 ap_count;
    
    policy = SL_SCAN_POLICY(1);         // SL_SCAN_ENABLE
    interval_sec = AF_SCAN_INTERVAL;
    
    // http://www.ti.com/lit/ug/swru368/swru368.pdf
    // after the scan interval is set, an immediate scan is activated
    sl_WlanPolicySet(SL_POLICY_SCAN, policy, (_u8 *)&interval_sec, sizeof(interval_sec));
    
    // http://e2e.ti.com/support/wireless_connectivity/f/968/t/354686.aspx
    // it takes about 900 msec to scan all the channels
    
    // we just block AF_AP_SCAN_DELAY ms to wait for scan finish, then we can retrieve scan results
    mico_thread_msleep(AF_AP_SCAN_DELAY);
    
    // result hardlimit: 20 ssid
    ap_count = sl_WlanGetNetworkList(0, AF_AP_SCAN_COUNT, sl_ap_list);

    // disable scan
    policy = SL_SCAN_POLICY(0);         // SL_SCAN_DISABLE
    sl_WlanPolicySet(SL_POLICY_SCAN, policy, 0, 0);
  
    return ap_count;
}
