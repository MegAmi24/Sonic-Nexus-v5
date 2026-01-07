cmake_minimum_required(VERSION 3.10)

project(SonicNexus)

set(GAME_NAME SonicNexus CACHE STRING "The mod directory to look into")
set(OUTPUT_NAME "Game" CACHE STRING "The name of the built library")

set(RETRO_REVISION 3 CACHE STRING "What revision to compile for. Defaults to Origins = 3")

option(RETRO_MOD_LOADER "Enables or disables the mod loader." ON)
set(RETRO_MOD_LOADER_VER 2 CACHE STRING "Sets the mod loader version. Defaults to latest")

option(GAME_INCLUDE_EDITOR "Whether or not to include editor functions. Defaults to true" ON)

option(GAME_INCREMENTAL_BUILD "Whether or not to build all objects separately (for quicker dev-->build iterations). Defaults to false." OFF)

if(GAME_INCREMENTAL_BUILD)
    include(${GAME_NAME}/Objects.cmake)
    set(GAME_SOURCES ${GENERATED_SOURCES})
else()
    set(GAME_SOURCES ${GAME_NAME}/Objects/All.cpp)
endif()

add_library(${GAME_NAME} SHARED
    RSDKv5-GameAPI/CPP/GameAPI/Game.cpp
    ${GAME_NAME}/SonicNexus.cpp
    ${GAME_SOURCES}
)

target_include_directories(${GAME_NAME} PRIVATE
    ${GAME_NAME}/
    ${GAME_NAME}/Objects/
    RSDKv5-GameAPI/CPP/
)

if(WIN32)
    target_compile_definitions(${GAME_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

set_target_properties(${GAME_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

target_compile_definitions(${GAME_NAME} PRIVATE
	RETRO_REVISION=${RETRO_REVISION}
	RETRO_USE_MOD_LOADER=$<BOOL:${RETRO_MOD_LOADER}>
	RETRO_MOD_LOADER_VER=${RETRO_MOD_LOADER_VER}
	GAME_INCLUDE_EDITOR=$<BOOL:${GAME_INCLUDE_EDITOR}>
	GAME_TYPE=0
	GAME_NO_GLOBALS=1
	GAME_CUSTOMLINKLOGIC=1
)

if(MSVC)
    target_link_options(${GAME_NAME} PRIVATE /OPT:NOICF)
endif()

unset(GAME_NAME CACHE)
unset(OUTPUT_NAME CACHE)