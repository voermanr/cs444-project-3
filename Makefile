all: eventbuf.c
	gcc -Wall -Wextra -g -o $@ $^ -lpthread

eventbuf: eventbuf.c
	gcc -Wall -Wextra -g -o $@ $^ -lpthread

clean:
	rm -f eventbuf