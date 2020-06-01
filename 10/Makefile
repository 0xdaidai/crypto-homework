CC=gcc
CFLAGS = -O -DDBG -D_REENTRANT -Wall -DUSE_PTHREAD -pthread
obj= DSA EIGamal rabin rabin-plus RSA-PSS schnorr
store=sha1.h sha256.h ari.h tools.h

all:$(obj)

$(obj): %: %.c
	$(CC) $(CFLAGS) $(store:%.h=%.c) $< -o $@ -lgmp

.PHONY : clean

clean :
	rm -f $(obj)
