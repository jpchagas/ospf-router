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
};

struct HELLO_HEADER {
    struct in_addr mask;
    u_int16_t helloint;
    u_int8_t options;
    u_int8_t priority;
    u_int32_t deadint;
    struct in_addr dr;
    struct in_addr bdr;
    struct in_addr neighbor[1];
};

struct DBD_HEADER {
	u_int16_t mtu;
	u_int8_t options;
	u_int8_t dbdescript;
	u_int32_t ddsequence;
};

struct LSA_HEADER_ACK{
	u_int16_t age;
    u_int8_t options;
    u_int8_t type_lsa;
    struct in_addr link_state_id;
    struct in_addr adv_router;
    u_int32_t seq;
    u_int16_t chksum;
    u_int16_t len;
}__attribute__((__packed__));

#endif
