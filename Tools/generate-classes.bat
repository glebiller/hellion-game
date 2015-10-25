@rem flatc.exe -j -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Common.fbs
@rem flatc.exe -j -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Definition.fbs
@rem flatc.exe -j -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Message.fbs

flatc.exe -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Systems/GraphicSystem.fbs
flatc.exe -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Systems/InputSystem.fbs
flatc.exe -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Systems/NetworkSystem.fbs
flatc.exe -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Systems/PhysicSystem.fbs

flatc.exe -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Systems.fbs
flatc.exe -c -o ../Core/Generated/Source -I ../Assets --gen-includes ../Assets/Schema/Environment.fbs

@rem ./protoc.exe --proto_path=../Assets --java_out=../Main/Server/src/main/java/ ../Assets/Proto/Common.proto
@rem ./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=./definition-encoder/src/main/java  ../Assets/Proto/Definition.proto
@rem ./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=../Main/Server/src/main/java/ ../Assets/Proto/Server/*.proto
@rem ./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source --java_out=../Main/Server/src/main/java/ ../Assets/Proto/Message.proto
@rem ./protoc.exe --proto_path=../Assets --cpp_out=../Core/Generated/Source ../Assets/Proto/Debug/*.proto

@rem flatc.exe -b -I ../Assets/Schema/ ../Assets/Schema/Definition.fbs ../Assets/Definitions/Application.json
