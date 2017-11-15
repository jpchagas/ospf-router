#include "ip.hpp"

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

void MontarHeaderIp(char* ipDestino, int tamanhoMensagem,
	struct OSPF_HEADER* ospf, struct LLS_HEADER* llsHeader,struct ip* ipHeader)
{
	ipHeader->ip_len = htons(sizeof(*llsHeader) + sizeof(*ipHeader) + sizeof(*ospf) + tamanhoMensagem);
	inet_pton(AF_INET, ipDestino, &(ipHeader->ip_dst));
	ipHeader->ip_id = htons(22227);
	ipHeader->ip_tos = 192;
	ipHeader->ip_sum = 0;
	ipHeader->ip_sum = CalculaChecksum ((uint16_t *) ipHeader, IP4_HDRLEN);
}
