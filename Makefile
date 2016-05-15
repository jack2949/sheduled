TEST_SRC     := $(shell /bin/pwd)
CJSON_DIR    := $(TEST_SRC)/cJSON

CFLAGS = -I$(CJSON_DIR)
CFLAGS += -fPIC -std=gnu99 -I./include 
CFLAGS += "-fno-stack-protector"
LDFLAGS = -Wl,--start-group -ldl -lpthread -Wl,--end-group
LDFLAGS += -L$(CJSON_DIR) -lcJSON -lm

OBJS = $(TEST_SRC)/src/main.o
OBJS += $(TEST_SRC)/src/log.o

TARGET = sheduled

.PHONY: all install clean
	
%.o: %.c Makefile *.h
	@echo  "  CC	$@"
	$(CC) -c $(CFLAGS) -o $@ $<
	
$(TARGET): $(OBJS)
	@echo  "  LD	$@"
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

all: $(TARGET)
	
install:
	@echo sheduled: installing sheduled ...
#	cp -f testd $(INSTALL_ROOT)/usr/bin/	
#	cp -f script/test $(INSTALL_ROOT)/etc/rc.d/init.d/
	
clean:
	@rm -f $(OBJS) $(TARGET)
