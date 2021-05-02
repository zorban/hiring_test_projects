objects = main.o

main: $(objects)
	g++ -o main $(objects) -lpthread

main.o: main.cpp main.h

.PHONY: clean
clean:
	-rm main $(objects)