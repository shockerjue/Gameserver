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
			$(PWD)/msghandle \
			$(PWD)/callback

SRC := $(PWD)/src

objs = main.o \
		$(SRC)/callback/callback.o \
		$(SRC)/gameserver.o \
		$(SRC)/keymanage.o \
		$(SRC)/chathost/chathost.o \
		$(SRC)/loginhost/loginhost.o \
		$(SRC)/dbhost/dbhost.o \
		$(SRC)/dbhost/dboperator.o \
		$(SRC)/role/role.o \
		$(SRC)/role/rolehost.o \
		$(SRC)/role/rolemanage.o \
		$(SRC)/eventhandle/event_manage.o \
		$(SRC)/msghandle/gameserver_msgheader.pb.o 
		

	

gameserver:$(objs)
	$(CXX) -o1 -o gameserver $(objs) $(CXXFLAGS) $(LDFLAGS)\
	 	-L $(PROTO_LIB) -lprotobuf -L$(ENGINELIB) -lengine -lpthread

install:
	cp gameserver $(INSTALL)

uninstall:
	rm $(INSTALL)/gameserver

clean:
	rm gameserver $(objs)
