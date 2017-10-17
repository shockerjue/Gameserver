LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
ROOT := $(PWD)/../..
PLATOM_NAME := $(shell uname -s)
CXXFLAGS :=

ifeq ($(PLATOM_NAME),$(APPLE))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include -I/usr/local/mysql/include
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/apple
	ENGINELIB := $(ROOT)/common/lib/apple
endif

ifeq ($(PLATOM_NAME),$(LINUX))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/linux
	ENGINELIB := $(ROOT)/common/lib/linux
endif


INSTALL := $(PWD)/bin
INCLUDE := $(PWD)/src \
			$(ROOT)/common/include \
			$(ROOT)/common/network/socket \
			$(ROOT)/common/util \
			$(ROOT)/msgdefine

SRC := $(PWD)/../src

objs = main.o \
		$(PWD)/thread_test.o \
		$(PWD)/shareram_test.o \
		$(PWD)/ramcache_test.o \
		$(PWD)/game_test.o \
		$(ROOT)/gateway/src/msghandle/gateway_msgheader.pb.o \
		$(ROOT)/gameserver/src/msghandle/gameserver_msgheader.pb.o	

	

main:$(objs)
	$(CXX) -o1 -o main $(objs) $(CXXFLAGS) $(LDFLAGS) \
		-L $(PROTO_LIB) -lprotobuf -L$(ENGINELIB) -lengine -lpthread

install:
	cp main $(INSTALL)

uninstall:
	rm $(INSTALL)/main

clean:
	rm main $(objs)
