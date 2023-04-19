pcseml: pcseml.c eventbuf.c
	gcc -Wall -Wextra -g -o $@ $^ -lpthread

clean:
	rm -f pcseml