#!/bin/sh
protoc --cpp_out=../Core/BaseTypes/Source/Proto --java_out=./DFEncoder/src/main/java ../Assets/Definitions/*.proto