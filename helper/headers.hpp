#include <sys/types.h>
#include <netinet/in.h>

#ifndef headersH
#define headersH

#define MAC_ADDR_LEN 6
#define	OSPF_AUTH_SIZE	8
#define IP4_HDRLEN 20
typedef unsigned char MacAddress[MAC_ADDR_LEN];

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

struct LLS_HEADER {
	u_int16_t checksum;
	u_int16_t len;
	u_int16_t type;
	u_int16_t optionsLen;
	u_int32_t options;
}__attribute__((__packed__));

struct OSPF_HEADER_CS_DBD {
	u_int8_t version;
    u_int8_t type;
    u_int16_t len;
    struct in_addr routerid;
    u_int32_t areaid;
    u_int16_t chksum;
    u_int8_t authdata[OSPF_AUTH_SIZE];

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

struct LSA_HEADER_NETWORK {
    u_int16_t age;
    u_int8_t options;
    u_int8_t type;
    struct in_addr stateid;
    struct in_addr router;
    u_int32_t seq;
    u_int16_t chksum;
    u_int16_t length;
    u_int32_t netmask;
    struct in_addr r1;
    struct in_addr r2;
}__attribute__((__packed__));

struct OSPF_HEADER_CS_LSU {
	u_int8_t version;
    u_int8_t type;
    u_int16_t len;
    struct in_addr routerid;
    u_int32_t areaid;
    u_int16_t chksum;
    u_int8_t authdata[OSPF_AUTH_SIZE];

	u_int32_t count;
	/*struct LSA_HEADER_ROUTER lsaRouter;*/
	struct LSA_HEADER_NETWORK lsaNetw;
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
} __attribute__((__packed__));

#endif
