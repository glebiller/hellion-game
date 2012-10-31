#!/bin/sh
protoc --proto_path=../Assets --cpp_out=../Core/BaseTypes/Source --java_out=./definition-encoder/src/main/java ../Assets/Proto/Common/*.proto ../Assets/Proto/Definition/*.proto
protoc --proto_path=../Assets --cpp_out=../Core/BaseTypes/Source --java_out=../Main/HelionServer/src/main/java/ ../Assets/Proto/Common/*.proto ../Assets/Proto/Server/*.proto ../Assets/Proto/Message/*.proto
