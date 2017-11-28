tunctl -t tap0
ifconfig tap0 172.16.4.2 netmask 255.255.255.252 up
iptables -t nat -A POSTROUTING -o wlp3s0 -j MASQUERADE
iptables -A FORWARD -i tap0 -j ACCEPT
route add -net 172.16.0.0 netmask 255.255.0.0 tap0
tunctl -t tap1
ifconfig tap1 172.16.7.2 netmask 255.255.255.252 up
iptables -t nat -A POSTROUTING -o wlp3s0taps -j MASQUERADE
iptables -A FORWARD -i tap1 -j ACCEPT
route add -net 172.16.7.0 netmask 255.255.255.0 tap1
