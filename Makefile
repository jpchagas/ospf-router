all:sniffer.c
	gcc sniffer.c -o sniffer
clean:
	rm -rf sniffer
