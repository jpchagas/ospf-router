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
	send_to();
}

void EnviaMensagemDbd(){
	MontarHeaderIp();
	MontarHeaderOspf();
	MontarHeaderDDB();
	send_to();
}

void EnviaMensagemLSAAck(){
	MontarHeaderIp();
	MontarHeaderOspf();
	MontarHeaderLSAAck();
	send_to();
}


int main()
{
	if((socketEnvio = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	{
		printf("Erro na criacao do socket.\n");
		exit(1);
	}

  int attempts = 0;
  EnviaMensagemHello(macLocal, macMulticast, 1);
	while(attempts < 10)
	{
    EnviaMensagemDbd(macLocal, macRoteador, 7);
	}
	//TODO: SLEEP DE 30 EM 30 SEGUNDOS E MANDAR UM PACOTE DDB. Com mensagem LSAACK DEPOIS

		EnviaMensagemLSAAck();

}
