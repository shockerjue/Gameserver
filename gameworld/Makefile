# CXXFLAGS += -std=c++11 -g -Ddebug 用于编程调式信息
LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
ROOT := $(PWD)/..
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

LIB := $(ROOT)/common/lib
INSTALL := $(PWD)/bin
INCLUDE := $(PWD)/src \
			$(ROOT)/common/include \
			$(ROOT)/common/network/socket \
			$(ROOT)/common/util \
			$(PWD)/callback

SRC := $(PWD)/src

objs = main.o \
		$(SRC)/callback/callback.o \
		$(SRC)/gamehost/gamehost.o \
		$(SRC)/eventhandle/event_manage.o \
		$(SRC)/threadhandle/threadhandle.o \
		$(SRC)/system_server/systemserver.o \
		$(SRC)/gameworld.o 		
	

gameworld:$(objs)
	$(CXX) -o1 -o gameworld $(objs) $(CXXFLAGS) $(LDFLAGS)\
		-L$(ENGINELIB) -lengine -lpthread -L $(PROTO_LIB) -lprotobuf 

install:
	cp gameworld $(INSTALL)

uninstall:
	rm $(INSTALL)/gameworld

clean:
	rm gameworld $(objs)