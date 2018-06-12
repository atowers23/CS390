program3: program3.o program3_utilities.o
	gcc -o program3 program3.o program3_utilities.o

program3.o: program3.c
	gcc -c program3.c

program3_utilities.o: program3_utilities.c program3_utilities.h
	gcc -c program3_utilities.c

clean:
	rm *.o

tar:
	tar -cvf program3.tar *.c *.h
