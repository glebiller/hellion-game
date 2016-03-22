flatc -b -o ../bin/Debug ../Framework/fbs/schema/environment.fbs ../Assets/Definitions/Environment.json
flatc -b -o ../bin/Debug -I ../Framework/fbs/ ../Framework/fbs/schema/scene.fbs ../Assets/Definitions/UniversalScene.json
flatc -b -o ../bin/Debug -I ../Framework/fbs/ ../Framework/fbs/schema/systems/graphic_system.fbs ../Assets/Definitions/Systems/GraphicSystem.json
