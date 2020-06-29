CC = gcc
store = MurmurHash2.c bloom.c aes.c
res = client server
CFLAGS = -DDBG -Wall -g -std=gnu99

all:$(res)
$(res):%:%.c
	$(CC) $(CFLAGS) $(store) $< -o $@ -lgmp -lm -lcjson

$(res):aes.h bloom.h murmurhash2.h file.h
.PHONY : clean

clean :
	rm -f $(res)
