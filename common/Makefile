LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
ROOT := $(PWD)/..
PLATOM_NAME := $(shell uname -s)
CXXFLAGS :=
INSTALL := $(PWD)

ifeq ($(PLATOM_NAME),$(APPLE))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include -I/usr/local/mysql/include
	TDARTY_LIB := $(ROOT)/3tdarty/lib/apple
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/apple
	INSTALL := $(PWD)/lib/apple
endif

ifeq ($(PLATOM_NAME),$(LINUX))
	CXXFLAGS += -std=c++11 -I$(ROOT)/3tdarty/protobuf/include
	TDARTY_LIB := $(ROOT)/3tdarty/lib/linux
	PROTO_LIB := $(ROOT)/3tdarty/protobuf/lib/linux
	INSTALL := $(PWD)/lib/linux
endif



INCLUDE := $(PWD)/src \
			$(SRC)/include \
			$(SRC)/network/socket \
			$(SRC)/util \
			$(SRC)/ddos \
			$(SRC)/engine

SRC := $(PWD)

objs =  $(SRC)/engine/networkmanage.o \
		$(SRC)/engine/callback.o \
		$(SRC)/engine/networkmanage.o \
		$(SRC)/src/value.o \
		$(SRC)/src/thread.o \
		$(SRC)/src/imodule.o \
		$(SRC)/src/cachemodule.o \
		$(SRC)/src/ramcache.o \
		$(SRC)/src/filecache.o \
		$(SRC)/src/cachemanage.o \
		$(SRC)/src/configmodule.o \
		$(SRC)/src/shareram.o \
		$(SRC)/src/servermodule.o \
		$(SRC)/src/environment.o \
		$(SRC)/util/log.o \
		$(SRC)/util/util.o \
		$(SRC)/util/datetime.o \
		$(SRC)/db/dbmodule.o \
		$(SRC)/db/mysqlutil.o \
		$(SRC)/db/redisutil.o \
		$(SRC)/msgdefine/msghandle.o \
		$(SRC)/msgdefine/msgheader.pb.o \
		$(SRC)/ipc/chat_db_ipc.pb.o \
		$(SRC)/ipc/gs_chat_ipc.pb.o \
		$(SRC)/ipc/gs_db_ipc.pb.o \
		$(SRC)/ipc/gs_gw_ipc.pb.o \
		$(SRC)/ipc/gs_login_ipc.pb.o \
		$(SRC)/ipc/gw_db_ipc.pb.o \
		$(SRC)/ipc/gy_login_ipc.pb.o \
		$(SRC)/ipc/login_db_ipc.pb.o \
		$(SRC)/ipc/ipc.pb.o \
		$(SRC)/network/socket/socket.o \
		$(SRC)/network/socket/socketaddress.o \
		$(SRC)/ddos/ddostype.o \
		$(SRC)/ddos/ddos.o \
		$(SRC)/ddos/timeout.o \
		$(SRC)/ddos/threadimplement.o \
		$(SRC)/engine/engine.o 

	
#gcc -shared -o libhellod.so hellod.c
libengine.a:$(objs)
	ar crv libengine.a $(objs) 
	cp libengine.a $(INSTALL)/

install:
	cp libengine.a $(INSTALL)/

uninstall:
	rm $(INSTALL)/lib/libengine.a

clean:
	rm libengine.a $(objs)