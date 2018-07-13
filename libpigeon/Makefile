all: libpigeon.so

clean:
	rm -f libpigeon.so pigeon.o pigeon_filter.o

libpigeon.so: pigeon.o pigeon_filter.o
	g++ -fPIC -shared -O2 pigeon.o pigeon_filter.o -o libpigeon.so -ldl

pigeon.o: pigeon.c
	gcc -fPIC -shared -O2 pigeon.c -c -o pigeon.o

pigeon_filter.o: pigeon_filter.cpp
	g++ -fPIC -shared -O2 pigeon_filter.cpp -c -o pigeon_filter.o
