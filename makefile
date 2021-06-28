

shooter: main.o ext.o fat16.o
	gcc main.o ext.o fat16.o -o shooter
main.o: main.c
	gcc -c main.c
ext.o: ext.c ext.h
	gcc -c ext.c
fat16.o: fat16.c fat16.h
	gcc -c fat16.c
clean:
	rm -f shooter *.o
