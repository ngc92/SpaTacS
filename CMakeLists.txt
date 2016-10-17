cmake_minimum_required(VERSION 3.5)
project(SpaTacS)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall)

find_package(Boost 1.58 REQUIRED)
find_library(Irrlicht_LIB Irrlicht)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(.
        ${Boost_INCLUDE_DIR})

## SOURCE FILES ##
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
        game/Starship.h
        core/Game.h
        core/Game.cpp
        core/GameState.h
        core/GameState.cpp
        game/Starship.cpp
        game/components/Engine.h
        game/components/Engine.cpp
        game/components/IWeapon.h
        game/components/ProjectileWeapon.cpp
        game/components/ProjectileWeapon.h
        game/components/RayWeapon.h
        game/components/RayWeapon.cpp
        game/Projectile.h
        game/Projectile.cpp
        game/components/IComponent.h
        game/components/IComponent.cpp
        game/components/ShieldGenerator.cpp
        game/components/ShieldGenerator.h
        core/GameThread.cpp
        core/GameThread.h
        game/GameObject.h
        game/GameObject.cpp
        game/components/PowerPlant.cpp
        game/components/PowerPlant.h
        core/Simulation.cpp
        core/Simulation.h
        game/Damage.h
        game/components/FuelTank.h
        game/components/FuelTank.cpp
        game/components/LifeSupport.cpp
        game/components/LifeSupport.h
        game/SubSystems.h
        game/SubSystems.cpp)

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
        physics/Object.cpp
        physics/Fixture.h
        physics/Fixture.cpp)

set(UI_SOURCE_FILES
        UI/IUI.h
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
        UI/inputs/UnitSelection.h
        UI/gfx/ShipStatusUI.cpp
        UI/gfx/ShipStatusUI.h
        UI/gfx/ShipFx.cpp
        UI/gfx/ShipFx.h
        UI/convert.h
        UI/convert.cpp
        UI/IrrRAII.h 
        UI/states/MainMenu.h 
        UI/states/MainMenu.cpp 
        UI/AppState.h 
        UI/states/GameState.h 
        UI/states/GameState.cpp 
        UI/states/SettingsMenu.h
        UI/states/SettingsMenu.cpp
        UI/StateManager.h 
        UI/StateManager.cpp 
        UI/gfx/MultiLineNode.h 
        UI/gfx/MultiLineNode.cpp)

set(SOURCE_FILES
        ${COMMAND_SOURCE_FILES}
        ${EVENT_SOURCE_FILES}
        ${CORE_SOURCE_FILES}
        ${PHYSICS_SOURCE_FILES}
        ${UI_SOURCE_FILES}
    main.cpp
       )

set(TEST_SOURCE_FILES test/vec_test.cpp test/collision_test.cpp)

add_executable(SpaTacS ${SOURCE_FILES})
add_executable(tests ${TEST_SOURCE_FILES} physics/HitTests.cpp)
target_link_libraries(SpaTacS ${Irrlicht_LIB} pthread)

## data files ##
set(DATA_SHEETS data/destroyer.xml)
