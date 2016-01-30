flatc -b ../Schema/Environment.fbs Environment.json
flatc -c -o Core/Generated/src --gen-includes Assets/Schema/Systems/GraphicSystem.fbs
flatc -o bin/Debug -b Assets/Schema/Systems/GraphicSystem.fbs Assets/Definitions/Systems/GraphicSystem.json