#
# Makefile
#
# Call 'make CROSSCOMPILE=' for native compilation
#

CLIENT=udpping
SERVER=udppingd
CSRCS=client.c
SSRCS=server.c

CROSSCOMPILE=/opt/crosstool-ng-powerpc/bin/powerpc-e500v2-linux-gnuspe-

default: $(CLIENT) $(SERVER)

$(CLIENT): $(CSRCS)
	$(CROSSCOMPILE)gcc -Wall $(CSRCS) -o $(CLIENT)

$(SERVER): $(SSRCS)
	$(CROSSCOMPILE)gcc -Wall $(SSRCS) -o $(SERVER)

clean:
	rm -rf *.o $(CLIENT) $(SERVER)

upload: $(CLIENT) $(SERVER)
	scp -P20160 $(CLIENT) $(SERVER) root@172.16.10.167:/tmp/
	scp -P20160 $(CLIENT) $(SERVER) root@172.16.10.168:/tmp/

.PHONY: clean upload
