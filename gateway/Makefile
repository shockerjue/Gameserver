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
			$(PWD)/src/msghandle \
			$(PWD)/callback

SRC := $(PWD)/src

objs = main.o \
		$(SRC)/msghandle/gateway_msgheader.pb.o \
		$(SRC)/callback/callback.o \
		$(SRC)/gateway.o \
		$(SRC)/loginhost/loginhost.o \
		$(SRC)/rolehost/rolehost.o \
		$(SRC)/platom/platom.o \
		$(SRC)/eventhandle/event_manage.o 
		

	

gateway:$(objs)
	$(CXX) -o1 -o gateway $(objs) $(CXXFLAGS) $(LDFLAGS)\
		-L $(PROTO_LIB) -lprotobuf -L$(ENGINELIB) -lengine -lpthread

install:
	cp gateway $(INSTALL)

uninstall:
	rm $(INSTALL)/gateway

clean:
	rm gateway $(objs)
