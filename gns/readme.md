### Configuration to be done in the host

#### Pre requisites

Execute the following command

	sudo apt-get install uml-utilities

#### Configuring a new Network Interface
Execute the commands below **one at a time**. The variable <**INTERFACE**> should be replaced by the interface connected to the internet in the host machine, example: *wlan0*

	sudo tunctl -t tap0
	sudo ifconfig tap0 172.16.4.2 netmask 255.255.255.252 up
	sudo iptables -t nat -A POSTROUTING -o <INTERFACE> -j MASQUERADE
	sudo iptables -A FORWARD -i tap0 -j ACCEPT
    sudo route add -net 172.16.0.0 netmask 255.255.0.0 tap0

##### Enabling the interface on reboot
Execute the command `crontab -e` to open crontab in a text editor.

Put the following lines in it:

	@reboot /usr/sbin/tunctl -t tap0 && /sbin/ifconfig tap0 172.16.4.2 netmask 255.255.255.252 up


#### Enabling ip forwarding
Execute the following command:

	sudo echo 1 > /proc/sys/net/ipv4/ip_forward

If it fails due to `Permission Denied`, try the one below:

	echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward

#### Validating the setup

* Open the topology in GNS3
* Start the Router **R2**
* The node **Host** should be started as well
* In the host machine, try pinging **R2**

	`ping 172.16.4.1`
