CC=g++
CFLAGS=-g -Wall
INC=-Iimage/ -Iutilities/
LIB=
BIN=
SRC=image/image.cpp utilities/utilities.cpp project1/project1.cpp
OBJ=$(SRC:.cpp=.o)
EXEC=hw1
LDFLAGS=


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CLFAGS) $(INC) -o $@ $^ $(LIB)
	
.cpp.o:
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)
