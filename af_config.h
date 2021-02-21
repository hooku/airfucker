#ifndef __AF_CONFIG_H_
#define __AF_CONFIG_H_

#define AF_DEBUG                1

#define AF_CHANNEL_MAX          13
#define AF_CHANNEL_HOP_INTVL    500     // ms
#define AF_HASH_TBL_SZ          60

#define AF_SCAN_INTERVAL        10      // sec
#define AF_AP_SCAN_DELAY        1000    // ms
#define AF_AP_SCAN_COUNT        10

#define AF_80211_MAX_PKT        1024    // byte
#define AF_80211_PREAMBLE       1       // 0 - short, 1 - long

#define AF_FCK_AP_COUNT         20
#define AF_FCK_SCAN_DURATION    20      // sec
#define AF_FCK_ATK_DURATION     20      // sec

#define VICTIM_LIST { \
    { 0x0c, 0x49,  51, 0x88, 0xc3, 0x11 }, \
    { 0xcc, 0x08, 251, 0x61, 0x03, 0x8e }, \
    { 0xcc, 0x08, 251, 0x61, 0x03, 0x90 }, \
    { 0x8c, 0xa6, 223, 0x4f, 0x65, 0x2a }, \
    { 0xdc, 0xfe,  24, 0x0b, 0x71, 0xd6 }, \
    { 0x78, 0x11, 220, 0x4b, 0x90, 0xb9 }, \
    { 0x88, 0x25, 147, 0x15, 0xb1, 0xc6 }, \C
    { 0xfc, 0x19, 208, 0x6d, 0x75, 0xb8 }, \
    { 0x78, 0x11, 220, 0x4b, 0x90, 0xba }  \
}

#endif // __AF_CONFIG_H_
