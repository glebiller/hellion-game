CC = g++
CFLAGS = -std=c++0x -Wall -Wno-unused-variable -Wno-unused-value
INCLUDES = -I~/Workspace/sandbox/boost_1_49_0 -I../BaseTypes/Source -I./Source
SRC_DIR = Source/
OBJ_DIR = Build/
OBJ_FILES = System.o Property.o
INSTALL_DIR = /usr/bin

all : $(OBJ_FILES)

clean :
	rm $(OBJ_FILES)

%.o: $(SRC_DIR)%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OBJ_DIR)$@ -c $<
