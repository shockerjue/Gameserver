LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
ROOT := $(PWD)/../..
PLATOM_NAME := $(shell uname -s)
CXXFLAGS :=

ifeq ($(PLATOM_NAME),$(APPLE))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include -I/usr/local/mysql/include
	TDARTY_LIB := $(ROOT)/3tdarty/lib/apple
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/apple
	ENGINELIB := $(ROOT)/common/lib/apple
endif

ifeq ($(PLATOM_NAME),$(LINUX))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include
	TDARTY_LIB := $(ROOT)/3tdarty/lib/linux
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/linux
	ENGINELIB := $(ROOT)/common/lib/linux
endif

INSTALL := $(PWD)/bin

INCLUDE := $(PWD)/src \
			$(SRC)/include \
			$(SRC)/network/socket \
			$(SRC)/util \
			$(SRC)/engine

SRC := $(PWD)../

objs =  main.o \
		ncallback.o


main:$(objs)
	$(CXX) -o1 -o main $(objs) $(CXXFLAGS) $(LDFLAGS) -L$(ENGINELIB) \
	 -lengine -lmysqlclient -L $(TDARTY_LIB) -lhiredis -lpthread -L $(PROTO_LIB) -lprotobuf

install:
	cp main $(INSTALL)

uninstall:
	rm $(INSTALL)/main

clean:
	rm main $(objs)