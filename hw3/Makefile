CC=g++
CFLAGS=-g -Wall -std=c++11
INC=-Iimage/ -Iutilities/ -Ihsi/
LIB=
BIN=
SRC=image/image.cpp utilities/utilities.cpp project/project.cpp hsi/hsi.cpp
OBJ=$(SRC:.cpp=.o)
EXEC=hw3
LDFLAGS=


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CLFAGS) $(INC) -o $@ $^ $(LIB)
	
.cpp.o:
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC) output-images/*.pgm output-images/*.ppm
