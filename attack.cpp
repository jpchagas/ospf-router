#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define MAC_ADDR_LEN 6
typedef unsigned char MacAddress[MAC_ADDR_LEN];

void MontarHeaderIp(){

}

void MontarHeaderOspf(){

}

void MontarHeaderHello(){

}

void MontarHeaderDDB(){

}

void MontarHeaderLSAAck(){

}


void EnviaMensagemHello(){
	MontarHeaderIp();
	MontarHeaderOspf();
	MontarHeaderHello();
	// sendto();
}

void EnviaMensagemDbd(){
	MontarHeaderIp();
	MontarHeaderOspf();
	MontarHeaderDDB();
	// sendto();
}

void EnviaMensagemLSAAck(){
	MontarHeaderIp();
	MontarHeaderOspf();
	MontarHeaderLSAAck();
	// sendto();
}


int main()
{
	int socketEnvio = 0;

	MacAddress macLocal = {0xa4, 0x1f, 0x72, 0xf5, 0x90, 0x80};
	MacAddress macMulticast = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x05};
	MacAddress macRoteador = {0x50, 0x3d, 0xe5, 0xd9, 0x07, 0xb8};

	if((socketEnvio = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		printf("Erro na criacao do socket.\n");
		exit(1);
	}

	int attempts = 0;
	// EnviaMensagemHello(macLocal, macMulticast, 1);
	EnviaMensagemHello();
	while(attempts < 10)
	{
		// EnviaMensagemDbd(macLocal, macRoteador, 7);
		EnviaMensagemDbd();
	}
		//TODO: SLEEP DE 30 EM 30 SEGUNDOS E MANDAR UM PACOTE DDB. Com mensagem LSAACK DEPOIS

	EnviaMensagemLSAAck();
	printf("End.\n");
}
