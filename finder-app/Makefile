.PHONY: all
all:
ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	aarch64-none-linux-gnu-gcc -g -Wall -c writer.c -o writer.o
	aarch64-none-linux-gnu-gcc -g -Wall -I/ writer.o -o writer
else
	gcc writer.c -o writer
endif

.PHONY: clean
clean:
	rm -f writer
	rm -f *.o
