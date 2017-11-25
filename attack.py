import socket, sys
from struct import *

def __init__(self):
    #create a raw socket
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    except socket.error , msg:
        print 'Socket could not be created. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
        sys.exit()

def begin():
    #socketEnvio = 0;

	#MacAddress macLocal = {0xa4, 0x1f, 0x72, 0xf5, 0x90, 0x80};
	#MacAddress macMulticast = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x05};
	#MacAddress macRoteador = {0x50, 0x3d, 0xe5, 0xd9, 0x07, 0xb8};


	#if((socketEnvio = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
	#{
	#	printf("Erro na criacao do socket.\n");
	#	exit(1);
	#}

	int attempts = 0;

	enviaMensagemHello();
	while(attempts < 10)
	{
		enviaMensagemDbd();
	}
		#TODO: SLEEP DE 30 EM 30 SEGUNDOS E MANDAR UM PACOTE DDB. Com mensagem LSAACK DEPOIS

	 #EnviaMensagemLSAAck();
	 printf("End.\n");

def build_ip_header():
    source_ip = '192.168.1.101'
    dest_ip = '192.168.1.1' # or socket.gethostbyname('www.google.com')

    # ip header fields
    ip_ihl =
    ip_ver =
    ip_tos =
    ip_tot_len =   # kernel will fill the correct total length
    ip_id =    #Id of this packet
    ip_frag_off =
    ip_ttl =
    ip_proto = socket.IPPROTO_TCP
    ip_check =     # kernel will fill the correct checksum
    ip_saddr = socket.inet_aton ( source_ip )   #Spoof the source ip address if you want to
    ip_daddr = socket.inet_aton ( dest_ip )

    ip_ihl_ver = (ip_ver << 4) + ip_ihl

    # the ! in the pack format string means network order
    ip_header = pack('!BBHHHBBH4s4s' , ip_ihl_ver, ip_tos, ip_tot_len, ip_id, ip_frag_off, ip_ttl, ip_proto, ip_check, ip_saddr, ip_daddr)
    return ip_header
def  build_ospf_header():
    ospf_version =
    ospf_type =
    ospf_len =
    ospf_routerid =
    ospf_areaid =
    ospf_chksum =
    ospf_authtype = 0
    ospf_auth = 0
    ospf_messagebody = 0
    ospf_header = pack('!BBHHHBBH4s4s' , ospf_version, ospf_type, ospf_len, ospf_routerid, ospf_areaid, ospf_chksum, ospf_authtype, ospf_auth, ospf_messagebody)
    return ospf_header
def build_ospf_hello_header():
	# inet_pton (AF_INET, ipBroadcast, &(hello.mask));
	mask = #ipBroadcast
	helloint = #htons(10);
	options = #18;
	priority = #1;
	deadint = #ntohl(40);
    #  Na primeria mensagem não há roteador backup
	if(primeiraMensagem)
		# inet_pton (AF_INET, "0.0.0.0", &(hello.dr));
		dr = #0.0.0.0
		# inet_pton (AF_INET, "0.0.0.0", &(hello.bdr));
		bdr = #0.0.0.0
	else
		# inet_pton (AF_INET, ipRoteador, &(hello.dr));
		dr = #ipRoteador
		# inet_pton (AF_INET, ipLocal, &(hello.bdr));
		bdr = #ipLocal
	# inet_pton (AF_INET, ipRoteador, &(hello.neighbor[0]));
	neighbor = #ipRoteador
    ospf_hello_header = pack('!BBHHHBBH4s4s' , mask,helloint)
    return ospf_hello_header
def build_ospf_dbd_header():

	# Configuração inicial DBD
	mtu = #htons(1500);
	options = #82;
	dbdescript = #7;
	ddsequence = #ntohl(seqDbScript);

	#configuracoes feitas no EnviaMensagemDbd
		#Configuração pacote Bdb
		#ddsequence = #htonl(seqDbScript);
		#dbdescript = #dbDescription;
    ospf_hello_header = pack('!BBHHHBBH4s4s' , )
    return ospf_dbd_header

def enviaMensagemHello():
    packet = build_ip_header() + build_ospf_header() + build_ospf_hello_header()
    s.sendto(packet, (dest_ip , 0 ))
def enviaMensagemDbd():
    packet = build_ip_header() + build_ospf_header() + build_ospf_dbd_header()
    s.sendto(packet, (dest_ip , 0 ))
def enviaMensagemLSAAck():
    packet = build_ip_header() + build_ospf_header() + build_ospf_lsaack_header()
    s.sendto(packet, (dest_ip , 0 ))

def checksum(msg):
    s = 0

    # loop taking 2 characters at a time
    for i in range(0, len(msg), 2):
        w = ord(msg[i]) + (ord(msg[i+1]) << 8 )
        s = s + w

    s = (s>>16) + (s & 0xffff);
    s = s + (s >> 16);

    #complement and mask to 4 byte short
    s = ~s & 0xffff

    return s


if __name__ == '__main__':
    """ Código do módulo secundário
    """
begin()
