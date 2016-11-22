cmake_minimum_required(VERSION 3.5)
project(SpaTacS)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall)

find_package(Boost 1.58 REQUIRED COMPONENTS filesystem)
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
        cmd/Attack.h
        cmd/Attack.cpp
        cmd/Move.h
        cmd/Move.cpp
        cmd/ICommand.h
        cmd/CommandManager.h
        cmd/CommandManager.cpp
        cmd/SetMode.h
        cmd/SetMode.cpp)

SET(CORE_SOURCE_FILES
        id.h
        game/Starship.h
        core/Game.h
        core/Game.cpp
        game/GameState.h
        game/GameState.cpp
        game/Starship.cpp
        game/Projectile.h
        game/Projectile.cpp
        game/components.cpp
        core/detail/GameThread.cpp
        core/detail/GameThread.h
        game/GameObject.h
        game/GameObject.cpp
        core/SimulationBase.cpp
        core/SimulationBase.h
        game/GameSimulation.cpp
        game/GameSimulation.h
        game/Damage.h
        game/SubSystems.h
        game/SubSystems.cpp
        game/components.h
        game/systems.h
        game/systems.cpp
        game/Damage.cpp
        game/LevelLoader.h
        game/LevelLoader.cpp
        game/AIPlayer.cpp
        game/AIPlayer.h
        game/ObjectID.h
        core/GameStateBase.h
        core/GameStateBase.cpp
        core/V2/fwd.h)

SET(PHYSICS_SOURCE_FILES
        vec.h
        physics/units.h
        physics/PhysicsWorld.h
        physics/PhysicsWorld.cpp
        physics/units.h
        physics/dimensions.h
        physics/HitTests.h
        physics/HitTests.cpp
        physics/Object.h
        physics/Object.cpp
        physics/Fixture.h
        physics/Fixture.cpp
        physics/parse_unit.h
        physics/parse_unit.cpp
        physics/ObjectID.h
        core/GameInterface.h)

set(UI_SOURCE_FILES
        UI/gfx/GridNode.h
        UI/gfx/GridNode.cpp
        UI/gfx/ShotFx.cpp
        UI/gfx/ShotFx.h
        UI/IrrlichtUI.cpp
        UI/IrrlichtUI.h
        UI/IInputMode.h
        UI/IInputMode.cpp
        UI/inputs/UnitCommand.cpp
        UI/inputs/UnitCommand.h
        UI/inputs/UnitSelection.cpp
        UI/inputs/UnitSelection.h
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
        UI/gfx/MultiLineNode.cpp
        UI/states/LevelSelection.h
        UI/states/LevelSelection.cpp
        UI/panels/ShipStatus.h
        UI/panels/ShipStatus.cpp
        UI/panels/DamageReport.h
        UI/panels/DamageReport.cpp
        UI/gfx/HealthBar.h
        UI/gfx/HealthBar.cpp
        UI/panels/WeaponsPanel.h
        UI/panels/WeaponsPanel.cpp)

set(SOURCE_FILES
        ${COMMAND_SOURCE_FILES}
        ${EVENT_SOURCE_FILES}
        ${CORE_SOURCE_FILES}
        ${PHYSICS_SOURCE_FILES}
        ${UI_SOURCE_FILES}
        main.cpp)

set(TEST_SOURCE_FILES test/vec_test.cpp test/collision_test.cpp test/units_test.cpp test/PhysicsWorld.cpp test/ecs_test.cpp)

add_executable(SpaTacS ${SOURCE_FILES})
add_executable(tests ${TEST_SOURCE_FILES} ${PHYSICS_SOURCE_FILES})
target_link_libraries(SpaTacS ${Irrlicht_LIB} ${Boost_FILESYSTEM_LIBRARY_DEBUG} ${Boost_SYSTEM_LIBRARY_DEBUG} pthread)

## data files ##
set(DATA_SHEETS data/destroyer.xml)
