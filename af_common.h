#define MAC_ADDR_LEN    6

#define SWAP_16(x)  (((x) >> 8) | ((x & 0x00FF) << 8))
#define SWAP_32(x)  (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

typedef struct __Mac_Addr_
{
    uint8_t     addr[MAC_ADDR_LEN];
} Mac_Addr;

typedef struct __ieee80211_header_
{
    uint16_t    ctl_frame;
    uint16_t    id_duration;
    Mac_Addr    addr_receiver;
    Mac_Addr    addr_transmitter;
    Mac_Addr    addr_destination;
    uint16_t    ctl_seq;
    uint8_t     extra[0];
} ieee80211_header;

typedef struct __ieee80211_mgmt_tag_header_
{
    uint8_t     tag_num;
    uint8_t     tag_len;
    uint8_t     tag_entity;
} ieee80211_mgmt_tag_header;

typedef enum _FRAME_TYPE
{
    /* Management */
    FT_MGT_BASE             = 0x00,
    FT_MGT_ASSOC_REQ        = 0x00,
    FT_MGT_ASSOC_RESP       = 0x10,
    FT_MGT_REASSOC_REQ      = 0x20,
    FT_MGT_REASSOC_RESP     = 0x30,
    FT_MGT_PROBE_REQ        = 0x40,
    FT_MGT_PROBE_RESP       = 0x50,
    FT_MGT_BEACON           = 0x80,
    FT_MGT_ATIM             = 0x90,
    FT_MGT_DISASSOC         = 0xA0,
    FT_MGT_AUTH             = 0xB0,
    FT_MGT_DEAUTH           = 0xC0,
    /* Control */
    FT_CTL_BASE             = 0x04,
    FT_CTL_PSPOOL           = 0xA4,
    FT_CTL_RTS              = 0xB4,
    FT_CTL_CTS              = 0xC4,
    FT_CTL_ACK              = 0xD4,
    FT_CTL_CF_END           = 0xE4,
    FT_CTL_CF_ENDACK        = 0xF4,
    /* Data */
    FT_DATA_BASE            = 0x08,
    FT_DATA                 = 0x08,
    FT_DATA_CF_ACK          = 0x18,
    FT_DATA_CF_POOL         = 0x28,
    FT_DATA_CF_ACKPOOL      = 0x38,
    FT_DATA_NULL            = 0x48,
    FT_DATA_NULL_CF_ACK     = 0x58,
    FT_DATA_NULL_CF_POOL    = 0x68,
    FT_DATA_NULL_CF_ACKPOOL = 0x78,
    FT_DATA_QOS             = 0x88,
} FRAME_TYPE;
