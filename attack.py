import socket, sys
from struct import *
import time

def __init__(self):
    #create a raw socket
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    except socket.error , msg:
        print 'Socket could not be created. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
        sys.exit()

def begin():
    sqn = 65000
	enviaMensagemHello(1, 44)
	while(true)
	{
        sqn  = sqn + 1
		enviaMensagemDbd(2, 32 , sqn)
        time.sleep(10)
	}

	 #enviaMensagemLSAAck()
	 printf("End.\n");

def build_ip_header(packetid):
    source_ip = '172.16.4.2'
    dest_ip = '172.16.4.1' # or socket.gethostbyname('www.google.com')

    # ip header fields
    ip_ihl = 5
    ip_ver = 4
    ip_tos = 192
    ip_tot_len = 0  # kernel will fill the correct total length
    ip_id = packetid   #Id of this packet
    ip_frag_off = 0
    ip_ttl = 255
    ip_proto = socket.IPPROTO_TCP
    ip_check = 0    # kernel will fill the correct checksum
    ip_saddr = socket.inet_aton ( source_ip )   #Spoof the source ip address if you want to
    ip_daddr = socket.inet_aton ( dest_ip )

    ip_ihl_ver = (ip_ver << 4) + ip_ihl

    # the ! in the pack format string means network order
    ip_header = pack('!BBHHHBBH4s4s' , ip_ihl_ver, ip_tos, ip_tot_len, ip_id, ip_frag_off, ip_ttl, ip_proto, ip_check, ip_saddr, ip_daddr)
    return ip_header
def build_ospf_header(packettype, packetlen):
    rid = '172.16.4.254'
    aid = '0.0.0.0'
    ospf_version = 2
    ospf_type = packettype
    ospf_len = packetlen
    ospf_routerid = socket.inet_aton ( rid )
    ospf_areaid = socket.inet_aton ( aid )
    ospf_chksum =
    ospf_authtype = 0
    ospf_auth = 0
    ospf_header = pack('!BBHIIHHQ' , ospf_version, ospf_type, ospf_len, ospf_routerid, ospf_areaid, ospf_chksum, ospf_authtype, ospf_auth)
    return ospf_header
def build_ospf_hello_header():
    m = '255.255.255.0'
    d = '172.16.4.1'
    b = '0.0.0.0'
	mask = socket.inet_aton(m)
	helloint = 10
	options = 2
	priority = 255
	deadint = 40
	dr = socket.inet_aton(d)
	bdr = socket.inet_aton(b)
	neighbor = 0
    ospf_hello_header = pack('!IHBBIIII', mask, helloint, options, priority, deadint, dr, bdr, neighbor)
    return ospf_hello_header
def build_ospf_dbd_header(sequencenumber):

	# Configuração inicial DBD
	mtu = 1500 #htons(1500);
	options = 2 #82;
	dbdescript = 7 #7;
	ddsequence = sequencenumber + 1#ntohl(seqDbScript);

	#configuracoes feitas no EnviaMensagemDbd
		#Configuração pacote Bdb
		#ddsequence = #htonl(seqDbScript);
		#dbdescript = #dbDescription;
    ospf_hello_header = pack('!HBBI' , mtu,options,dbdescript,ddsequence)
    return ospf_dbd_header

def enviaMensagemHello(packettype, packetlen):
    packet = build_ip_header() + build_ospf_header(packettype, packetlen) + build_ospf_hello_header()
    s.sendto(packet, (dest_ip , 0 ))
def enviaMensagemDbd(packettype, packetlen, sequencenumber):
    packet = build_ip_header() + build_ospf_header(packettype, packetlen) + build_ospf_dbd_header(sequencenumber)
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
