bin.out: client.o
	gcc -Iinclude/ -g -o bin.out client.o

client.o: client.cpp
	gcc -Iinclude/ -g -c client.cpp

clean:
	rm -f *.o *.out