CFLAGS = -Wall -std=c99

all: testme

testme: testme.c
	gcc -o $@ $< ${CFLAGS}
	./testme

clean:
	rm -rf testme
