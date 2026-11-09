#include "tusb.h"

#define _PID_MAP(itf, n) ((CFG_TUD_##itf) << (n))
#define USB_PID           (0x4000 | _PID_MAP(AUDIO, 1))

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0x303A,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

enum {
    ITF_NUM_AUDIO_CONTROL = 0,
    ITF_NUM_AUDIO_STREAMING_SPK,
    ITF_NUM_AUDIO_STREAMING_MIC,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + CFG_TUD_AUDIO * (9 + 8 + 8))

uint8_t const desc_fs_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 500),
    0x09, TUSB_DESC_INTERFACE, ITF_NUM_AUDIO_CONTROL, 0x00, 0x00, TUSB_CLASS_AUDIO, AUDIO_SUBCLASS_CONTROL, AUDIO_INT_PROTOCOL_CODE_V2, 0x00,
    0x09, TUSB_DESC_INTERFACE, ITF_NUM_AUDIO_STREAMING_SPK, 0x00, 0x00, TUSB_CLASS_AUDIO, AUDIO_SUBCLASS_STREAMING, AUDIO_INT_PROTOCOL_CODE_V2, 0x00,
    0x09, TUSB_DESC_INTERFACE, ITF_NUM_AUDIO_STREAMING_MIC, 0x00, 0x00, TUSB_CLASS_AUDIO, AUDIO_SUBCLASS_STREAMING, AUDIO_INT_PROTOCOL_CODE_V2, 0x00
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_fs_configuration;
}

static char const* string_desc_arr [] = {
    (const char[]) { 0x09, 0x04 },
    "Kniajevich DSP",
    "P4 Faust Hybrid DSP Soundcard",
    "P4-FAUST-GOLDEN-01",
    "Faust Audio Out",
    "Faust Audio In"
};

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    static uint16_t _desc_str[64];
    uint8_t chr_count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0]))) return NULL;
        const char* str = string_desc_arr[index];
        chr_count = (uint8_t) strlen(str);
        if (chr_count > 63) chr_count = 63;
        for (uint8_t i = 0; i < chr_count; i++) _desc_str[1+i] = str[i];
    }
    _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2*chr_count + 2));
    return _desc_str;
}

bool tud_audio_set_req_entity_cb(uint8_t rhport, tusb_control_request_t const *p_request, uint8_t *p_buff) {
    (void) rhport; (void) p_request; (void) p_buff;
    return true;
}

bool tud_audio_get_req_entity_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
    (void) rhport; (void) p_request;
    return true;
}

bool tud_audio_set_itf_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
    (void) rhport; (void) p_request;
    return true;
}

bool tud_audio_set_itf_close_cb(uint8_t rhport, tusb_control_request_t const *p_request) {
    (void) rhport; (void) p_request;
    return true;
}

bool tud_audio_rx_done_post_read_cb(uint8_t rhport, uint16_t n_bytes_received, uint8_t func_id, uint8_t ep_out, uint8_t cur_alt_setting) {
    (void) rhport; (void) n_bytes_received; (void) func_id; (void) ep_out; (void) cur_alt_setting;
    return true;
}

bool tud_audio_tx_done_pre_load_cb(uint8_t rhport, uint8_t func_id, uint8_t ep_in, uint8_t cur_alt_setting) {
    (void) rhport; (void) func_id; (void) ep_in; (void) cur_alt_setting;
    return true;
}
