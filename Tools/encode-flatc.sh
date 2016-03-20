flatc -c -o ../Core/Generated/include --no-prefix ../Assets/Schema/EntityChange.fbs

flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/SystemType.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/SystemComponentType.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/Environment.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/UniversalScene.fbs
flatc -c -o ../Core/Generated/include --scoped-enums ../Assets/Schema/Systems/GraphicSystem.fbs
flatc -c -o ../Core/Generated/include --scoped-enums --gen-mutable ../Assets/Schema/Component/GraphicSystemComponents.fbs
flatc -c -o ../Core/Generated/include --scoped-enums --gen-mutable ../Assets/Schema/Component/InputSystemComponents.fbs
flatc -c -o ../Core/Generated/include --scoped-enums --gen-mutable ../Assets/Schema/Component/PhysicSystemComponents.fbs

flatc -b -o ../bin/Debug ../Core/Framework/fbs/schema/environment.fbs ../Assets/Definitions/Environment.json
flatc -b -o ../bin/Debug -I ../Core/Framework/fbs/ ../Core/Framework/fbs/schema/scene.fbs ../Assets/Definitions/UniversalScene.json
flatc -b -o ../bin/Debug -I ../Core/Framework/fbs/ ../Core/Framework/fbs/schema/systems/graphic_system.fbs ../Assets/Definitions/Systems/GraphicSystem.json
