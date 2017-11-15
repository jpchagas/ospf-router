#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <string.h>
#include "headers.hpp"
#include "ip.hpp"

void MontarHeaderOspf(int tipo, int tamanhoMensagem,
	struct OSPF_HEADER* ospf, struct OSPF_HEADER_CS_DBD* ospCSDbd, struct OSPF_HEADER_CS_LSU* ospCSLsu, struct LSA_HEADER_ACK* lsuAck)
{
	ospf->type = tipo;
	ospCSDbd->type = ospf->type;
	ospf->len = htons(sizeof(*ospf) + tamanhoMensagem);

	ospCSDbd->len = ospf->len;
	ospCSLsu->len = ospf->len;
	lsuAck->len = ospf->len;
}

void MontarDadosPadraoMensagem(MacAddress mac_L, MacAddress mac_D,
	struct ip* ipHeader, struct OSPF_HEADER* ospf,
	char* buffer, short int* etherTypeT)
{
	memcpy(buffer, mac_D, MAC_ADDR_LEN);
	memcpy((buffer + MAC_ADDR_LEN), mac_L, MAC_ADDR_LEN);
	memcpy((buffer + (2 * MAC_ADDR_LEN)), etherTypeT, sizeof(*etherTypeT));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(*etherTypeT), ipHeader, IP4_HDRLEN);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(*etherTypeT) + IP4_HDRLEN, ospf, sizeof(*ospf));
}

void MontarHeaderHello(MacAddress mac_L, MacAddress mac_D, int primeiraMensagem,
	struct sockaddr_ll* destAddr, struct HELLO_HEADER* hello,
	struct ip* ipHeader,
	struct OSPF_HEADER* ospf, struct OSPF_HEADER_CS_DBD* ospCSDbd, struct OSPF_HEADER_CS_LSU* ospCSLsu, struct LSA_HEADER_ACK* lsuAck,
	struct LLS_HEADER* llsHeader,
	char* ipBroadcast, char* ipLocal, char* ipRoteador, char* ipMulticast,
	char* buffer, short int* etherTypeT)
{
	memcpy(&(destAddr->sll_addr), mac_D, MAC_ADDR_LEN);

	MontarHeaderIp(ipMulticast, sizeof(*hello), ospf, llsHeader, ipHeader);
	MontarHeaderOspf(1, sizeof(*hello), ospf, ospCSDbd, ospCSLsu, lsuAck);

	// Configuração pacote Hello
	inet_pton (AF_INET, ipBroadcast, &(hello->mask));
	hello->helloint = htons(10);
	hello->options = 18;
	hello->priority = 1;
	hello->deadint = ntohl(40);

    // Na primeria mensagem não há roteador backup
	if(primeiraMensagem)
	{
		inet_pton (AF_INET, "0.0.0.0", &(hello->dr));
		inet_pton (AF_INET, "0.0.0.0", &(hello->bdr));
	}
	else
	{
		inet_pton (AF_INET, ipRoteador, &(hello->dr));
		inet_pton (AF_INET, ipLocal, &(hello->bdr));
	}
	inet_pton (AF_INET, ipRoteador, &(hello->neighbor[0]));

	MontarDadosPadraoMensagem(mac_L, mac_D, ipHeader, ospf, buffer, etherTypeT);
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(*etherTypeT) + IP4_HDRLEN + sizeof(*ospf), hello, sizeof(*hello));
	memcpy((buffer + (2 * MAC_ADDR_LEN)) + sizeof(*etherTypeT) + IP4_HDRLEN + sizeof(*ospf) + sizeof(*hello), llsHeader, sizeof(*llsHeader));

	// We are only mounting it here
	// Envio da mensagem
	// if((retornoEnvio = sendto(socketEnvio, buffer, 14 + sizeof(llsHeader) + IP4_HDRLEN + sizeof(ospf) + sizeof(hello),
	// 	0, (struct sockaddr *)&(destAddr), sizeof(struct sockaddr_ll))) < 0)
	// {
	// 	printf("Mensagem Hello - Erro! \n");
	// 	exit(1);
	// }
	//
	// printf("Mensagem Hello - %d\n", retornoEnvio);
}
