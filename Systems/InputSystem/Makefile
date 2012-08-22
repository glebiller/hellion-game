CC = g++
CFLAGS = -std=c++0x -Wall -Wno-unused-variable -Wno-unused-value -Wno-unknown-pragmas
INCLUDES = -I../../../boost_1_49_0 -I../../../ois_1_3/includes  -I../../Externs/OISB/Source -I../../Externs/ProtocolBuffers/Source -I../../Core/BaseTypes/Source -I../../Core/Interfaces/Source -I../../Core/Framework/Source -I./Source
SRC_DIR = Source/
OBJ_DIR = Build/
OBJ_FILES = System.o Scene.o Task.o Object/Object.o Object/MouseObject.o Object/ObjectCamera.o Object/ObjectPlayer.o
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

Object/%.o: $(SRC_DIR)Object/%.cpp
	$(cc-command)
