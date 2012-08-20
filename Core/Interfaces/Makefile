CC = g++
CFLAGS = -std=c++0x -Wall -Wno-unused-variable -Wno-unused-value
INCLUDES = -I~/Workspace/sandbox/boost_1_49_0 -I../BaseTypes/Source -I./Source -I../../Externs/ProtocolBuffers/Source
SRC_DIR = Source/
OBJ_DIR = Build/
OBJ_FILES = System/ISystem.o System/ISystemSubject.o System/ISystemScene.o System/ISystemObject.o Observer/CSubject.o Property/IProperty.o
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

System/%.o: $(SRC_DIR)System/%.cpp
	$(cc-command)
