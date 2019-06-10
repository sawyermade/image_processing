CC=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config --cflags opencv`
INC=-Iimage/ -Iutilities/ -Ihsi/ -Iopencv/
LIB=`pkg-config --libs opencv`
BIN=
SRC=image/image.cpp utilities/utilities.cpp project/project.cpp hsi/hsi.cpp opencv/opencv.cpp
OBJ=$(SRC:.cpp=.o)
EXEC=hw4
LDFLAGS=


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CLFAGS) $(INC) -o $@ $^ $(LIB)
	
.cpp.o:
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC) output-images/*.pgm output-images/*.ppm
