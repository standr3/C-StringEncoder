build:encoder.c
	gcc -Wall encoder.c -o encoder
run:build
	./encoder
clean:
	rm -f encoder

