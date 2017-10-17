ROOT := $(shell pwd)
INCLUDE := $(ROOT)/include
LIB := $(ROOT)/common/lib

USR_SUB_DIR :=  $(ROOT)/common \
				$(ROOT)/gateway \
				$(ROOT)/gateway/test \
				$(ROOT)/common/network \
				$(ROOT)/common/test \
				$(ROOT)/login/test \
				$(ROOT)/login \
				$(ROOT)/gameserver \
				$(ROOT)/gameserver/test \
				$(ROOT)/gameworld \
				$(ROOT)/gameworld/test \
				$(ROOT)/dataaccess \
				$(ROOT)/dataaccess/test \
				$(ROOT)/chatserver \
				$(ROOT)/chatserver/test \
				$(ROOT)/tool/test \
				$(ROOT)/3tdarty/protobuf/test \
				$(ROOT)/logserver \
				$(ROOT)/logserver/test

default:usr

usr:
	@date
	@for n in $(USR_SUB_DIR); do $(MAKE) -C $$n ; done
	@echo 'build sucess...............!'
	@date
	
clean:
	@for n in $(USR_SUB_DIR); do $(MAKE) -C $$n clean; done
	@echo "clean file sucess.............!"

install:
	@for n in $(USR_SUB_DIR); do $(MAKE) -C $$n install; done
	@echo "install success..................!"

uninstall:
	@for n in $(USR_SUB_DIR); do $(MAKE) -C $$n uninstall; done
	@echo "uninstall success.................!"