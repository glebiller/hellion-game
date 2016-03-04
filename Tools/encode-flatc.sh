flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/SystemType.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/SystemComponentType.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/Environment.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/UniversalScene.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/Systems/GraphicSystem.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/Component/GraphicSystemComponents.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/Component/PhysicSystemComponents.fbs

flatc -b -o ../bin/Debug ../Assets/Schema/Environment.fbs ../Assets/Definitions/Environment.json
flatc -b -o ../bin/Debug ../Assets/Schema/UniversalScene.fbs ../Assets/Definitions/UniversalScene.json
flatc -b -o ../bin/Debug ../Assets/Schema/Systems/GraphicSystem.fbs ../Assets/Definitions/Systems/GraphicSystem.json
