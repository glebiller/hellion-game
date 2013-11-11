#!/bin/sh
./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=./definition-encoder/src/main/java ../Assets/Proto/Common.proto
./protoc.exe --proto_path=../Assets --java_out=../Main/Server/src/main/java/ ../Assets/Proto/Common.proto
./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=./definition-encoder/src/main/java ../Assets/Proto/Definition.proto
./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=../Main/Server/src/main/java/ ../Assets/Proto/Server/*.proto
./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=../Main/Server/src/main/java/ ../Assets/Proto/Message.proto
./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source ../Assets/Proto/Debug/*.proto
