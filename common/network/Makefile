CXXFLAGS += -std=c++11

PWD := $(shell pwd)
ROOT := $(PWD)/..

SRC := $(PWD)/socket
INSTALL := $(PWD)/bin

objs = test.o $(SRC)/socketaddress.o $(SRC)/socket.o $(ROOT)/util/log.o \
	$(ROOT)/util/datetime.o

test:$(objs)
	$(CXX) -o1 -o main $(objs) $(CXXFLAGS) $(LDFLAGS)

install:
	cp main $(INSTALL)

uninstall:
	rm $(INSTALL)/main

clean:
	rm main $(objs)
