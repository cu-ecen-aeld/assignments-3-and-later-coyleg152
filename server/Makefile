.PHONY: all
all:
ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	aarch64-none-linux-gnu-gcc -g -Wall -c aesdsocket.c -o aesdsocket.o
	aarch64-none-linux-gnu-gcc -g -Wall -I/ aesdsocket.o -o aesdsocket
else
	gcc aesdsocket.c -o aesdsocket -Wall -g
endif

default:
ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	aarch64-none-linux-gnu-gcc -g -Wall -c aesdsocket.c -o aesdsocket.o
	aarch64-none-linux-gnu-gcc -g -Wall -I/ aesdsocket.o -o aesdsocket
else
	gcc aesdsocket.c -o aesdsocket -Wall -g
endif

.PHONY: clean
clean:
	rm -f aesdsocket
	rm -f *.o
