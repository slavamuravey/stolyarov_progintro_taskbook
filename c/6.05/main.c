#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

union ip {
    uint32_t i;
    uint8_t octs[4];
};

int main(int argc, char **argv)
{
    char *ip_str;
    union ip ip;
    uint16_t buf[4];
    int i;
    
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    ip_str = argv[1];
    
    int matches = sscanf(ip_str, "%hu.%hu.%hu.%hu", &buf[3], &buf[2], &buf[1], &buf[0]);

    if (matches != 4) {
        fprintf(stderr, "Can't parse ip address.\n");
        exit(1);
    }

    for (i = 0; i < sizeof(buf) / sizeof(*buf); i++) {
        uint16_t oct = buf[i];
        if (oct < 1 || oct > 255) {
            fprintf(stderr, "Bad ip address.\n");
            exit(1);
        }

        ip.octs[i] = oct;
    }

    for (i = 0; i < 33; i++) {
        uint32_t mask;
        union ip subnet;
        mask = 0xffffffffUL << (32 - i);
        subnet.i = ip.i;
        subnet.i &= mask;
        printf("%hu.%hu.%hu.%hu/%d\n", subnet.octs[3], subnet.octs[2], subnet.octs[1], subnet.octs[0], i);
    }

    return 0;
}
