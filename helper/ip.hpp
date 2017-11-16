#include <netinet/ip.h>
#include <arpa/inet.h>
#include "headers.hpp"
#ifndef ipH
#define ipH

unsigned short CalculaChecksum(unsigned short *addr, int len);
void ipHeaderInitialSetup(struct ip* ipHeader, char* ipLocal, char* ipMulticast);
void MontarHeaderIp(char* ipDestino, int tamanhoMensagem,
	struct OSPF_HEADER* ospf, struct LLS_HEADER* llsHeader,struct ip* ipHeader);

#endif
