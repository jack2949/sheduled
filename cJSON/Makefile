#INSTALL = $(INSTALL_ROOT)
LIBS   = libcJSON.so
LIBOBJS  = cJSON.o

CFLAGS= -I$./
LDFLAGS= -Wall -Wl,-allow-shlib-undefined
CFLAGS += -fPIC




.PHONY: all
all: $(LIBS)
	
%.o: %.c Makefile *.h
	@echo  "  CC	$@"
	$(CC) -c $(CFLAGS) -o $@ $<	
	
$(LIBS): $(LIBOBJS)
	@echo  "  LD	$@"
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -shared -o $@	
	
.PHONY: install
install:
	#@cp -f $(LIBS)   $(INSTALL)/lib/

clean:
	rm -f $(LIBOBJS) $(LIBS)
