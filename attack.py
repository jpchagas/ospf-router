import socket, sys, time, threading
from struct import *

# def __init__(self):
#     try:
#         s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
#     except socket.error , msg:
#         print 'Socket could not be created. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
#         sys.exit()

source_ip = '172.16.4.2'
dest_ip = '172.16.4.1'
sqn = 40000

def begin():
    global sqn
    print "Enviando Mensagem Hello OSPF"
    enviaMensagemHello(1, 48, sqn)
    i = 0
    while(i<10):
        sqn  = sqn + 1
        print "Enviando Mensagem DBD OSPF"
        if i==0:
            enviaMensagemDbd(2, 32 , sqn, 7)
        elif i==9:
            enviaMensagemDbd(2, 32 , sqn, 1)
        else:
            enviaMensagemDbd(2, 32 , sqn, 3)
        i+=1
        time.sleep(1)

    keepAlive()

	 #enviaMensagemLSAAck()
     #print 'End.\n'

def build_ip_header(packetid):
    global source_ip
    global dest_ip
    # ip header fields
    ip_ihl = 5
    ip_ver = 4
    ip_tos = 192
    ip_tot_len = 0  # kernel will fill the correct total length
    ip_id = packetid   #Id of this packet
    ip_frag_off = 0
    ip_ttl = 255
    ip_proto = 89
    ip_check = 0    # kernel will fill the correct checksum
    ip_saddr = socket.inet_aton ( source_ip )   #Spoof the source ip address if you want to
    ip_daddr = socket.inet_aton ( dest_ip )

    ip_ihl_ver = (ip_ver << 4) + ip_ihl

    # the ! in the pack format string means network order
    ip_header = pack('!BBHHHBBH4s4s' , ip_ihl_ver, ip_tos, ip_tot_len, ip_id, ip_frag_off, ip_ttl, ip_proto, ip_check, ip_saddr, ip_daddr)
    return ip_header
def build_ospf_header(packettype, packetlen, checksuma):
    rid = '172.16.4.2'
    aid = '0.0.0.0'
    ospf_version = 2
    ospf_type = packettype
    ospf_len = packetlen
    ospf_routerid = ip2int(rid)
    ospf_areaid = ip2int(aid)
    ospf_chksum = checksuma
    ospf_authtype = 0
    ospf_auth = 0
    ospf_header = pack('!BBHIIHHQ' , ospf_version, ospf_type, ospf_len, ospf_routerid, ospf_areaid, ospf_chksum, ospf_authtype, ospf_auth)
    return ospf_header
def build_ospf_hello_header():
    m = '255.255.255.0'
    d = '172.16.4.1'
    b = '0.0.0.0'
    mask = ip2int(m)
    helloint = 10
    options = 2
    priority = 255
    deadint = 40
    dr = ip2int(d)
    bdr = ip2int(b)
    ospf_hello_header = pack('!IHBBIIII', mask, helloint, options, priority, deadint, dr, bdr, dr)
    return ospf_hello_header
def build_ospf_dbd_header(sequencenumber, dbvalormestre):
    # Configuracao inicial DBD
    mtu = 1500 #htons(1500);
    options = 2 #82;
    dbdescript = dbvalormestre #7;
    ddsequence = sequencenumber + 1
    #ntohl(seqDbScript);
    #configuracoes feitas no EnviaMensagemDbd
    #Configuracao pacote Bdb
    #ddsequence = #htonl(seqDbScript);
    #dbdescript = #dbDescription;
    ospf_dbd_header = pack('!HBBI' , mtu,options,dbdescript, ddsequence)
    return ospf_dbd_header

def enviaMensagemHello(packettype, packetlen, sequencenumber):
    global dest_ip
    packet = build_ospf_header(packettype, packetlen, 0) + build_ospf_hello_header()
    ck = checksum(packet)
    packet = build_ip_header(sequencenumber) + build_ospf_header(packettype, packetlen, ck) + build_ospf_hello_header()
    s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    s.sendto(packet, (dest_ip , 0 ))
def enviaMensagemDbd(packettype, packetlen, sequencenumber, dbvalormestre):
    global dest_ip
    packet = build_ospf_header(packettype, packetlen, 0) + build_ospf_dbd_header(sequencenumber, dbvalormestre)
    ck = checksum(packet)
    packet = build_ip_header(sequencenumber) + build_ospf_header(packettype, packetlen,ck) + build_ospf_dbd_header(sequencenumber, dbvalormestre)
    s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
    s.sendto(packet, (dest_ip , 0 ))


def checksum(msg):
    s = 0
    # loop taking 2 characters at a time
    for i in range(0, len(msg), 2):
        if i>=9 or i<=12:
            pass
        w = ord(msg[i]) + (ord(msg[i+1]) << 8 )
        s = s + w

    s = (s>>16) + (s & 0xffff);
    s = s + (s >> 16);

    #complement and mask to 4 byte short
    s = ~s & 0xffff
    # alfa = s & 0xff00
    # beta = s & 0x00ff
    # beta = beta
    # alfa = alfa
    # s = (beta | alfa) & 0xffff
    s = swap32(s)
    s = (s >> 16)
    print hex(s)
    return s

def swap32(i):
    return unpack("<I", pack(">I", i))[0]

def ip2int(addr):
    return unpack("!I", socket.inet_aton(addr))[0]

def keepAlive():
    global sqn
    threading.Timer(10.0, keepAlive).start()
    print "Sending keep alive"
    sqn = sqn + 1
    enviaMensagemHello(1, 48, sqn)

if __name__ == '__main__':
    begin()
