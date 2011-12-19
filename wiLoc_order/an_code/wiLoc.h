#include "../client/wifi_status.h"
#include <math.h>
#define POWER_NUMBER 20//from 0 dbm to 100 dbm every 5dbm
#define MAX_AP_NUMBER 100
#define MAX_NORMALIZED 0x00ffffffffffffff
#define LOG(A,B) (log(A)/log(B))

typedef struct AP_spec_t{
    long long int mac;
    long long int power[POWER_NUMBER];
} AP_spec;

typedef struct AP_Order_t{
	long long int mac[MAX_AP_NUMBER];
	int order[MAX_AP_NUMBER][MAX_AP_NUMBER];
} AP_order;

typedef struct _SCORE{
	char name[256];
	double score;
}AP_SCORE;

int wiLoc_index(int argc,char *argv[],char *result);
int wiLoc_position(AP_info * ap,int apn,char *result);
int wiLoc_order(AP_info *ap,int apn,char *logname,char *result);
