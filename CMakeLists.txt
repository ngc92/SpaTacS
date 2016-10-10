cmake_minimum_required(VERSION 3.5)
project(SpaTacS)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

find_library(Boost 1.58 REQUIRED)





set(EVENT_SOURCE_FILES
        events/Accelerate.h
        events/Accelerate.cpp
        events/ShipEvent.h
        events/ShipEvent.cpp
        events/Combat.h
        events/Combat.cpp
        events/Spawn.h
        events/IEvent.h
        events/Spawn.cpp)

set(COMMAND_SOURCE_FILES
        UI/cmd/Commands.h
        UI/cmd/Attack.h
        UI/cmd/Attack.cpp
        UI/cmd/Move.h
        UI/cmd/Move.cpp
        UI/cmd/ICommand.h
        UI/cmd/CommandManager.h
        UI/cmd/CommandManager.cpp
        UI/cmd/SetMode.h
        UI/cmd/SetMode.cpp)

SET(CORE_SOURCE_FILES
        core/Starship.h
        core/Game.h
        core/Game.cpp
        core/GameState.h
        core/GameState.cpp
        core/Starship.cpp
        core/components/Engine.h
        core/components/Engine.cpp
        core/components/IWeapon.h
        core/components/ProjectileWeapon.cpp
        core/components/ProjectileWeapon.h
        core/components/RayWeapon.h
        core/components/RayWeapon.cpp
        core/Projectile.h
        core/Projectile.cpp
        core/components/IComponent.h
        core/components/IComponent.cpp
        core/components/ShieldGenerator.cpp
        core/components/ShieldGenerator.h
        core/GameThread.cpp
        core/GameThread.h
        core/GameObject.h
        core/GameObject.cpp
        core/components/PowerPlant.cpp
        core/components/PowerPlant.h
        core/Simulation.cpp
        core/Simulation.h
        core/Damage.h core/components/FuelTank.h core/components/FuelTank.cpp core/components/LifeSupport.cpp core/components/LifeSupport.h UI/IrrRAII.h core/SubSystems.h core/SubSystems.cpp)

SET(PHYSICS_SOURCE_FILES
        vec.h
        physics/units.h
        physics/PhysicsWorld.h
        physics/Events.h
        physics/PhysicsWorld.cpp
        physics/Events.cpp
        physics/units.h
        physics/dimensions.h
        physics/HitTests.h
        physics/HitTests.cpp
        physics/Object.h
        physics/Object.cpp )

set(UI_SOURCE_FILES
        UI/IUI.h
        UI/ConsoleUI.h
        UI/ConsoleUI.cpp
        UI/gfx/GridNode.h
        UI/gfx/GridNode.cpp
        UI/gfx/ShipUI.h
        UI/gfx/ShotFx.cpp
        UI/gfx/ShotFx.h
        UI/IrrlichtUI.cpp
        UI/IrrlichtUI.h
        UI/IInputMode.h
        UI/IInputMode.cpp
        UI/inputs/UnitCommand.cpp
        UI/inputs/UnitCommand.h
        UI/AIPlayer.cpp
        UI/AIPlayer.h
        UI/inputs/UnitSelection.cpp
        UI/inputs/UnitSelection.h UI/gfx/ShipStatusUI.cpp UI/gfx/ShipStatusUI.h UI/gfx/ShipFx.cpp UI/gfx/ShipFx.h UI/convert.h UI/convert.cpp)

set(SOURCE_FILES
        ${COMMAND_SOURCE_FILES}
        ${EVENT_SOURCE_FILES}
        ${CORE_SOURCE_FILES}
        ${PHYSICS_SOURCE_FILES}
        ${UI_SOURCE_FILES}
    main.cpp
       )

set(TEST_SOURCE_FILES test/vec_test.cpp test/collision_test.cpp)


include_directories(.
        Boost_INCLUDE_DIRS)

add_executable(SpaTacS ${SOURCE_FILES})
add_executable(tests ${TEST_SOURCE_FILES} physics/HitTests.cpp)
target_link_libraries(SpaTacS Irrlicht pthread)
