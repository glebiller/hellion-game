CC = g++
CFLAGS = -std=c++0x -Wall -Wno-unused-variable -Wno-unused-value
INCLUDES = -I~/Workspace/sandbox/boost_1_49_0 -I./Source
SRC_DIR = Source/
OBJ_DIR = Build/
OBJ_FILES = Debug.o Singleton.o MathUtils.o
INSTALL_DIR = /usr/bin

define cc-command
mkdir -p $(OBJ_DIR)$(@D)
$(CC) $(CFLAGS) $(INCLUDES) -o $(OBJ_DIR)$@ -c $<
endef

all : $(OBJ_FILES)

clean :
	rm $(OBJ_FILES)

%.o: $(SRC_DIR)%.cpp
	$(cc-command)
