CC=clang++
CFLAGS=
OUTPUT=syrhackuse

all: build

build:
	$(CC) -o $(OUTPUT) $(CFLAGS) main.cpp

clean:
	rm ../tests/result.* $(OUTPUT) 

# uses feh to view the output image
fehview: build 
	./$(OUTPUT)
	feh out
