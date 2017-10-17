LINUX := Linux
APPLE := Darwin
PWD := $(shell pwd)
ROOT := $(PWD)/..
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


LIB := $(ROOT)/common/lib
INSTALL := $(PWD)/bin
INCLUDE := $(PWD)/src \
			$(ROOT)/common/include \
			$(ROOT)/common/network/socket \
			$(ROOT)/common/util \
			$(ROOT)/common/db \
			$(PWD)/callback \
			$(PWD)/dbrw \
			$(PWD)/cache \
			$(PWD)/io-thread

SRC := $(PWD)/src

objs = main.o \
		$(SRC)/callback/callback.o \
		$(SRC)/dataaccess.o \
		$(SRC)/dbhost/dbhost.o \
		$(SRC)/eventhandle/event_manage.o \
		$(SRC)/dbrw/mysqlslave.o \
		$(SRC)/dbrw/mysqlmaster.o \
		$(SRC)/system_server/systemserver.o \
		$(SRC)/io-thread/io_thread.o \
		$(SRC)/io-thread/common.o \
		$(SRC)/io-thread/read.o \
		$(SRC)/io-thread/sync.o \
		$(SRC)/io-thread/write.o \
		$(SRC)/baghost/baghost.o \
		$(SRC)/chathost/chathost.o \
		$(SRC)/sharecache/sharecache.o
		

	

dataaccess:$(objs)
	$(CXX) -o1 -o dataaccess $(objs) $(CXXFLAGS) $(LDFLAGS) \
		-L$(ENGINELIB) -lengine -lmysqlclient -L $(TDARTY_LIB) \
		 -lhiredis -lpthread -L $(PROTO_LIB) -lprotobuf
		
install:
	cp dataaccess $(INSTALL)

uninstall:
	rm $(INSTALL)/dataaccess

clean:
	rm dataaccess $(objs)
