LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
PLATOM_NAME := $(shell uname -s)
ROOT := $(PWD)/..
CXXFLAGS :=

ifeq ($(PLATOM_NAME),$(APPLE))
	CXXFLAGS += -std=c++11 -I$(ROOT)/include -I/usr/local/mysql/include
	LIB := $(ROOT)/lib/apple
endif

ifeq ($(PLATOM_NAME),$(LINUX))
	CXXFLAGS += -std=c++11 -I$(ROOT)/include
	LIB := $(ROOT)/lib/linux
endif


INSTALL := $(PWD)/bin

INCLUDE := $(PWD) \
			$(ROOT)/include 

SRC := $(PWD)

objs =  $(SRC)/main.o \
		$(SRC)/student.pb.o

	
#gcc -shared -o libhellod.so hellod.c
main:$(objs)
	$(CXX) -o1 -o main $(objs) $(CXXFLAGS) $(LDFLAGS)  -L $(LIB) -lprotobuf

install:
	cp main $(INSTALL)

uninstall:
	rm $(INSTALL)/main


clean:
	rm main $(objs)
