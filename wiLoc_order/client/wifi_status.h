#include <stdio.h>
#include <stdint.h>
typedef struct AP_info_t{
    uint64_t mac;
    int quality;
    int level;
} AP_info;
typedef struct AP_head_t{
    int AP_num;
    int cmd;
    int posID;
} AP_head;
int wifi_status(AP_info* pinfo);
