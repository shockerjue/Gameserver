LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
ROOT := $(PWD)/..
PLATOM_NAME := $(shell uname -s)
CXXFLAGS :=

ifeq ($(PLATOM_NAME),$(APPLE))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include -I/usr/local/mysql/include
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/apple
	TDARTY_LIB := $(ROOT)/3tdarty/lib/apple
	ENGINELIB := $(ROOT)/common/lib/apple
endif

ifeq ($(PLATOM_NAME),$(LINUX))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/linux
	ENGINELIB := $(ROOT)/common/lib/linux
	TDARTY_LIB := $(ROOT)/3tdarty/lib/linux
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
		$(SRC)/chatserver.o \
		$(SRC)/eventhandle/event_manage.o \
		$(SRC)/chathandle/chathandle.o \
		$(SRC)/gshost/gshost.o \
		$(SRC)/dbhost/dbhost.o \
		$(SRC)/playerpool/playerpool.o \
		$(SRC)/system_server/systemserver.o 

	

chatserver:$(objs)
	$(CXX) -o1 -o chatserver $(objs) $(CXXFLAGS) $(LDFLAGS) -L$(ENGINELIB) \
	 -lengine -lmysqlclient -L $(TDARTY_LIB) -lhiredis -lpthread  -L $(PROTO_LIB) -lprotobuf

install:
	cp chatserver $(INSTALL)

uninstall:
	rm $(INSTALL)/chatserver

clean:
	rm chatserver $(objs)
