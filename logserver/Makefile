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
			$(PWD)/cache

SRC := $(PWD)/src

objs = main.o \
		$(SRC)/callback/callback.o \
		$(SRC)/logserver.o \
		$(SRC)/logrw/logread.o \
		$(SRC)/logrw/logwrite.o \
		$(SRC)/logrw/logrw.o \
		$(SRC)/loghost/loghost.o \
		$(SRC)/eventhandle/event_manage.o \
		$(SRC)/system_server/systemserver.o 
		

	

logserver:$(objs)
	$(CXX) -o1 -o logserver $(objs) $(CXXFLAGS) $(LDFLAGS) \
		-L$(ENGINELIB) -lengine -lmysqlclient -L $(TDARTY_LIB) \
		 -lhiredis -lpthread -L $(PROTO_LIB) -lprotobuf
		
install:
	cp logserver $(INSTALL)

uninstall:
	rm $(INSTALL)/logserver

clean:
	rm logserver $(objs)
