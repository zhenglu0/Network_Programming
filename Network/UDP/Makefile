CC=gcc
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: udp-send udp-recv udp-client udp-server

udp-send: udp-send.o 
	$(CC) -o udp-send udp-send.o $(LIBS)

udp-recv: udp-recv.o 
	$(CC) -o udp-recv udp-recv.o $(LIBS)

udp-send.o: udp-send.c port.h

udp-recv.o: udp-recv.c port.h

udp-client: udp-client.o 
	$(CC) -o udp-client udp-client.o $(LIBS)

udp-server: udp-server.o 
	$(CC) -o udp-server udp-server.o $(LIBS)

udp-client.o: udp-client.c port.h

udp-server.o: udp-server.c port.h

clean:
	rm -f udp-send udp-recv udp-send.o udp-recv.o udp-client udp-server udp-client.o udp-server.o 
