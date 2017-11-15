#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <string.h>
#include "headers.hpp"
#include "ip.hpp"

#ifndef ospfH
#define ospfH

void MontarHeaderOspf(int tipo, int tamanhoMensagem,
	struct OSPF_HEADER* ospf, struct OSPF_HEADER_CS_DBD* ospCSDbd, struct OSPF_HEADER_CS_LSU* ospCSLsu, struct LSA_HEADER_ACK* lsuAck);

void MontarDadosPadraoMensagem(MacAddress mac_L, MacAddress mac_D,
	struct ip* ipHeader, struct OSPF_HEADER* ospf,
	char* buffer, short int* etherTypeT);

void MontarHeaderHello(MacAddress mac_L, MacAddress mac_D, int primeiraMensagem,
	struct sockaddr_ll* destAddr, struct HELLO_HEADER* hello,
	struct ip* ipHeader,
	struct OSPF_HEADER* ospf, struct OSPF_HEADER_CS_DBD* ospCSDbd, struct OSPF_HEADER_CS_LSU* ospCSLsu, struct LSA_HEADER_ACK* lsuAck,
	struct LLS_HEADER* llsHeader,
	char* ipBroadcast, char* ipLocal, char* ipRoteador, char* ipMulticast,
	char* buffer, short int* etherTypeT);

#endif
