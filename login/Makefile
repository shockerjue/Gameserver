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
	ENGINELLIB := $(ROOT)/common/lib/apple
endif

ifeq ($(PLATOM_NAME),$(LINUX))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/linux
	TDARTY_LIB := $(ROOT)/3tdarty/lib/linux
	ENGINELLIB := $(ROOT)/common/lib/linux
endif

LIB := $(ROOT)/common/lib
INSTALL := $(PWD)/bin
INCLUDE := $(PWD)/src \
			$(ROOT)/common/include \
			$(ROOT)/common/network/socket \
			$(ROOT)/common/util \
			$(PWD)/callback \
			$(PWD)/db \
			$(PWD)/event_manage \
			$(PWD)/gameserver \
			$(PWD)/gateway

SRC := $(PWD)/src

objs = main.o \
		$(SRC)/callback/datacallback.o \
		$(SRC)/login.o \
		$(SRC)/dbhost/dbhandle.o \
		$(SRC)/gameserver/gameserverhandle.o \
		$(SRC)/gateway/gatewayhandle.o \
		$(SRC)/eventhandle/event_manage.o \
		$(SRC)/system_server/systemserver.o 

	

login:$(objs)
	$(CXX) -o1 -o login $(objs) $(CXXFLAGS) $(LDFLAGS) -L$(ENGINELLIB)\
	 -lengine -lmysqlclient -L $(TDARTY_LIB) -lhiredis -lpthread -L $(PROTO_LIB) -lprotobuf

install:
	cp login $(INSTALL)

uninstall:
	rm $(INSTALL)/login

clean:
	rm login $(objs)
