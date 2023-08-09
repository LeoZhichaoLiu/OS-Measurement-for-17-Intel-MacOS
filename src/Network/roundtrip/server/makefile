bin.out: server.o
	gcc -Iinclude/ -g -o bin.out server.o

server.o: server.cpp
	gcc -Iinclude/ -g -c server.cpp

clean:
	rm -f *.o *.out