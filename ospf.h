#ifndef OSPF_H
#define OSPF_H

struct OSPF_HEADER {
    u_int8_t version;
    u_int8_t type;
    u_int16_t len;
    struct in_addr routerid;
    u_int32_t areaid;
    u_int16_t chksum;
    u_int16_t authtype;
    u_int8_t authdata[OSPF_AUTH_SIZE];
}__attribute__((__packed__));

#endif
