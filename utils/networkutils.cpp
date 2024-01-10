#include "networkutils.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>
#include "common/log.h"

#define MAX_INTERFACES 64
#define PING_COUNT 1
#define PING_TIMEOUT 3

char *ipAddr = nullptr;

static int pingTest(const char *ipAddr) {
    char command[100];
    sprintf(command, "ping -c %d -w %d %s > /dev/null 2>&1", PING_COUNT, PING_TIMEOUT, ipAddr);
    return system(command);
}

NetworkUtils::NetworkUtils()
{

}

const char *NetworkUtils::getOwnIpaddr()
{
    struct ifaddrs *ifaddr, *ifa;

    int connected = 0;

    if (getifaddrs(&ifaddr) == -1) {
        RLOGE("getifaddrs failed");
        goto RET;
    }

    RLOGD("List of network interfaces:\n");

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && (ifa->ifa_flags & IFF_UP)) {
            RLOGD("%s\n", ifa->ifa_name);
            if (ifa->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
                ipAddr = inet_ntoa(sa->sin_addr);
                RLOGD("IP Address: %s\n", ipAddr);

                if (strcmp(ipAddr, "127.0.0.1") != 0) {
                    RLOGD("Trying to ping %s...\n", ipAddr);
                    if (pingTest(ipAddr) == 0) {
                        RLOGD("Connected to internet through interface: %s\n", ifa->ifa_name);
                        connected = 1;
                        break;
                    }
                }
            }
        }
    }

    if (!connected) {
        RLOGD("No internet connection detected.\n");
    }

RET:
    if(ifaddr)
        freeifaddrs(ifaddr);

    return ipAddr;
}
