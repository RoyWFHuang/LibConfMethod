ifneq ("$(wildcard ./config.mk)", "")
include config.mk
endif

ifeq ($(CFLAG),)
  SRC_ROOT = $(pwd)/src
endif

ifeq ($(CONFIGMETHOD_LIB_NAME),)
	CONFIGMETHOD_LIB_NAME = libconfigmethod.a
endif

ifeq ($(ERROR_MSG_MODE), yes)
	CFLAG = -DERROR_MSG_MODE
endif

ifeq ($(DEBUG_MODE), yes)
    CFLAG += -DDEBUG_MODE
    ifeq ($(LCM_DEBUG_MODE), yes)
        CFLAG += -DLCM_DEBUG_MODE
    endif
endif

ifeq ($(SYSLOG), yes)
CFLAG += -DSYSLOG
endif

CONFIGMETHOD_FILE = src/lib_config_method.c

CONFIGMETHOD_LIB_FILE = $(CONFIGMETHOD_FILE)

INCLUDE_DIR = \
-I./inc/ \
-I./h/

all:
	$(CC) -c $(CONFIGMETHOD_LIB_FILE) $(CFLAG) $(INCLUDE_DIR)
	ar -r $(CONFIGMETHOD_LIB_NAME) *.o
	mv $(CONFIGMETHOD_LIB_NAME) ./lib/
	rm -rf *.o

clean:
	rm -rf *.o ./lib/$(CONFIGMETHOD_LIB_NAME)

distclean: clean
	rm -rf lcmtest

test:
	$(CC) -c $(CONFIGMETHOD_LIB_FILE) lcm_test.c $(CFLAG) \
$(INCLUDE_DIR) $(DEBUG_FLAG) -DCONSOLE_DEBUG
	$(CC) -g *.o -lcrypto -o lcmtest -lpthread -lconfig
	rm -rf *.o
