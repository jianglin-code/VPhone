#define LOG_TAG "CustomMadeService"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <cutils/log.h>
#include <cutils/properties.h>

//using namespace android;

/*static char* get_local_ip()
{
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;
	sock = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sock == -1) {
		ALOGD("socket");
		return NULL;
	}

	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
		ALOGD("ioctl");
		return NULL;
	}

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	return inet_ntoa(sin.sin_addr);
}

static void config_vm_net_work(void)
{
	errno = 0;
	int ret = system("ifconfig eth0 down");
	ALOGD("ifconfig eth0 down errno = %s", strerror(errno));

	ret = system("ip link set eth0 name wlan0");
	ALOGD("ip link set eth0 name wlan0 errno = %s", strerror(errno));

	char value[PROPERTY_VALUE_MAX];
	property_get("ro.custommade.deviceinfo.mac", value, "ac:c1:ee:57:64:9b");
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd), "ifconfig wlan0 hw ether %s", value);
	ret = system(cmd);
	ALOGD("%s errno = %s", cmd, strerror(errno));

	ret = system("ifconfig wlan0 up");
	ALOGD("ifconfig wlan0 up errno = %s", strerror(errno));
};*/

/*int calculateLuhn(const char* number14) {
    int sum = 0;
    for (int i = 0; i < 14; ++i) {
        int digit = number14[i] - '0';
        if (i % 2 == 1) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
    }
    return (10 - (sum % 10)) % 10;
}

void generateIMEI(char* imeiOut) {
    const char* tac = "86838703";
    char snr[6] = {0};

    for (int i = 0; i < 6; ++i) {
        snr[i] = '0' + rand() % 10;
    }

    char first14[15] = {0};
    strcpy(first14, tac);
    strcat(first14, snr);

    int checkDigit = calculateLuhn(first14);
    sprintf(imeiOut, "%s%d", first14, checkDigit);
}

void generateAndroidID(char* idOut) {
	const char* s = "0123456789abcdef";
    for (int i = 0; i < 16; ++i) {
        int index = rand() % 16;
        idOut[i] = s[index];
    }
}*/

int main(int /*argc*/, char** /*argv*/)
{
    ALOGI("GuiExt service start...");

	const char* initfile = "/data/local/tmp/custommade.conf";
	if (access(initfile, R_OK) != 0) {
		/*char imei[16] = {0};
		char androidid[17] = {0};

		srand((unsigned int) time(NULL));
	
		generateIMEI(imei);
		generateAndroidID(androidid);

		property_set("persist.ro.custommade.deviceinfo.imei", imei);
		property_set("persist.ro.custommade.deviceinfo.androidid", androidid);*/

		char value[PROPERTY_VALUE_MAX] = {0};
		property_get("ro.boot.vm", value, "1");
		if (strcmp(value, "0") == 0) {
			property_set("persist.sys.host.selinux.enforce", "1");
		}

		property_set("persist.sys.custommade.partition.rw", "0");

		int fd = open(initfile, O_WRONLY|O_CREAT, 0660);
		if (fd >= 0) {
			write(fd, "1", strlen("1"));
			close(fd);
		}
	}

    ALOGD("CustomMade service exit...");
    return 0;
}
