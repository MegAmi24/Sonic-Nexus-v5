cmake_minimum_required(VERSION 3.10)

project(SonicNexus)

set(GAME_NAME SonicNexus CACHE STRING "The mod directory to look into")
set(OUTPUT_NAME "SonicNexus" CACHE STRING "The name of the built library")

set(RETRO_REVISION 3 CACHE STRING "What revision to compile for. Defaults to Origins = 3")

option(RETRO_MOD_LOADER "Enables or disables the mod loader." ON)
set(RETRO_MOD_LOADER_VER 2 CACHE STRING "Sets the mod loader version. Defaults to latest")

option(GAME_INCLUDE_EDITOR "Whether or not to include editor functions. Defaults to true" ON)

file(GLOB OBJECTS RELATIVE ${CMAKE_SOURCE_DIR} ${GAME_NAME}/Objects/*/*.cpp)

add_library(SonicNexus SHARED
    RSDKv5-GameAPI/CPP/GameAPI/Game.cpp
    ${GAME_NAME}/SonicNexus.cpp
    ${GAME_NAME}/Math.cpp
    ${OBJECTS}
)

target_include_directories(SonicNexus PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
    RSDKv5-GameAPI/CPP/
)

set_target_properties(SonicNexus PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

target_compile_definitions(SonicNexus PRIVATE
	RETRO_REVISION=${RETRO_REVISION}
	RETRO_USE_MOD_LOADER=$<BOOL:${RETRO_MOD_LOADER}>
	RETRO_MOD_LOADER_VER=${RETRO_MOD_LOADER_VER}
	GAME_INCLUDE_EDITOR=$<BOOL:${GAME_INCLUDE_EDITOR}>
	GAME_TYPE=0
	GAME_NO_GLOBALS=1
	GAME_CUSTOMLINKLOGIC=1
)

unset(GAME_NAME CACHE)
unset(OUTPUT_NAME CACHE)