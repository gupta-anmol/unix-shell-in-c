main: main.c
	gcc main.c -o main && ./main

run:
	./main

clean:
	rm -f history.txt main
