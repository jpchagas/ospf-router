GXX= g++
CPFLAGS= -std=c++11 -lpthread
SRC= attack.cpp
BIN= ospf_attack.run

all:
	$(GXX) -o $(BIN) $(SRC) $(CPFLAGS)
clean:
	rm -rf $(BIN)
