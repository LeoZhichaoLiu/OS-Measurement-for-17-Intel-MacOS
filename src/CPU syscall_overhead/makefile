bin.out: main.o
	gcc -Iinclude/ -g -o bin.out main.o

main.o: main.cpp
	gcc -Iinclude/ -g -c main.cpp

clean:
	rm -f *.o *.out