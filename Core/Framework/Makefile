CC = g++
CFLAGS = -std=c++0x -Wall -Wno-unused-variable -Wno-unused-value -Wno-unknown-pragmas
INCLUDES = -I../../../boost_1_49_0 -I../../../tbb_4_0_5/include -I../../Externs/ProtocolBuffers/Source -I../BaseTypes/Source -I../Interfaces/Source -I./Source
SRC_DIR = Source/
OBJ_DIR = Build/
OBJ_FILES = Manager/EnvironmentManager.o Manager/MemoryManager.o Manager/PlatformManager.o Manager/ServiceManager.o Manager/SystemManager.o Manager/TaskManager.o Scheduler.o
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

Manager/%.o: $(SRC_DIR)Manager/%.cpp
	$(cc-command)
