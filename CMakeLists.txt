cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

# PC target only. The IDO N64 ROM still builds with Makefile.
# libultraship pin: Kenix3/libultraship @ 60c9bb713ed0850da66559fc17c556d03da649cd
# (SpaghettiKart ce0f9358f0505bbdeb76b1aff7a25556cda3fdef)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use")

if(WIN32)
  option(ENABLE_VCPKG "Enable vcpkg for dependencies" ON)
else()
  option(ENABLE_VCPKG "Enable vcpkg for dependencies" OFF)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/AmpedUp")
  set(PC_EXE AmpedUp)
  set(PC_HOST_DIR src/AmpedUp)
  set(PC_WINDOW_NAME "Amped Up")
  set(PC_WINDOW_ID ampedup)
  option(PC_LINK_GAME "Compile mk64/OverKart/Amped Up C into the PC exe" ON)
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/OverKart5")
  set(PC_EXE OverKart5)
  set(PC_HOST_DIR src/OverKart5)
  set(PC_WINDOW_NAME "OverKart5")
  set(PC_WINDOW_ID overkart5)
  option(PC_LINK_GAME "Compile mk64/OverKart C into the PC exe" OFF)
else()
  set(PC_EXE LibraryDecomp)
  set(PC_HOST_DIR src)
  set(PC_WINDOW_NAME "LibraryDecomp")
  set(PC_WINDOW_ID librarydecomp)
  option(PC_LINK_GAME "Compile mk64/Library C into the PC exe" OFF)
endif()

project(${PC_EXE} VERSION 1.0.0 LANGUAGES C CXX)

if(ENABLE_VCPKG)
  include(cmake/dependencies/vcpkg.cmake)
endif()

include(libultraship/cmake/cvars.cmake)

# Match this tree's GRUCODE=f3dex_old (F3DEX 0.95).
set(GBI_UCODE F3DEX_GBI CACHE STRING "GBI ucode version" FORCE)
set(EXCLUDE_MPQ_SUPPORT TRUE CACHE BOOL "")
set(INCLUDE_MPQ_SUPPORT OFF CACHE BOOL "")

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PC_EXE})

if(WIN32)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(libultraship ${CMAKE_BINARY_DIR}/libultraship)

set(PC_SOURCES
  src/pc/Engine.cpp
  src/pc/Game.cpp
  src/pc/GBIMiddleware.cpp
  src/pc/libultra_stubs.c
  src/pc/rom.cpp
  src/pc/game_stubs.c
)
if(NOT PC_LINK_GAME)
  list(APPEND PC_SOURCES src/pc/audio_stub.c)
endif()

add_executable(${PC_EXE} ${PC_SOURCES})

target_include_directories(${PC_EXE} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/pc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/racing
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ending
  ${CMAKE_CURRENT_SOURCE_DIR}/src/audio
  ${CMAKE_CURRENT_SOURCE_DIR}/src/debug
  ${CMAKE_CURRENT_SOURCE_DIR}/src/buffers
  ${CMAKE_CURRENT_SOURCE_DIR}/src/os
  ${CMAKE_CURRENT_SOURCE_DIR}/src/OverKartLibrary
  ${CMAKE_CURRENT_SOURCE_DIR}/${PC_HOST_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/${PC_HOST_DIR}/DisplayLists
  ${CMAKE_CURRENT_SOURCE_DIR}/${PC_HOST_DIR}/3DPlayerModel
  ${CMAKE_CURRENT_SOURCE_DIR}/src/OverKartLibrary/CustomCourses
  ${CMAKE_CURRENT_SOURCE_DIR}/src/OverKartLibrary/CustomObjects
  ${CMAKE_CURRENT_SOURCE_DIR}/src/OverKartLibrary/GameVariables/NTSC
  ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/include
  ${CMAKE_CURRENT_SOURCE_DIR}/libultraship/include/libultraship
)

target_compile_definitions(${PC_EXE} PRIVATE
  VERSION_US=1
  F3DEX_GBI=1
  F3D_OLD=1
  AVOID_UB=1
  NON_MATCHING=1
  _LANGUAGE_C
  _USE_MATH_DEFINES
  EXCLUDE_MPQ_SUPPORT
  CIMGUI_DEFINE_ENUMS_AND_STRUCTS
)

if(WIN32)
  target_compile_definitions(${PC_EXE} PRIVATE
    WIN32
    _CRT_SECURE_NO_WARNINGS
    NOMINMAX
    UNICODE
    _UNICODE
    ENABLE_DX11
    # Game HUD vs Windows GDI
    SetTextColor=Mk64_SetTextColor
  )
endif()

if(PC_LINK_GAME)
  target_compile_definitions(${PC_EXE} PRIVATE PC_LINK_GAME=1)
  file(GLOB GU_C "${CMAKE_CURRENT_SOURCE_DIR}/src/os/gu*.c")
  target_sources(${PC_EXE} PRIVATE
    src/main.c
    src/pc/audio_stub.c
    ${GU_C}
  )
endif()

target_compile_definitions(libultraship PRIVATE F3DEX_GBI=1 F3D_OLD=1)

add_dependencies(${PC_EXE} libultraship)
target_link_libraries(${PC_EXE} PRIVATE libultraship)

if(MSVC)
  set_property(TARGET ${PC_EXE} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_compile_options(${PC_EXE} PRIVATE /MP /permissive- /wd4244 /wd4267 /wd4305 /wd4018 /wd4101 /wd4996)
  # Asset/ROM symbols are not in-tree; LUS also defines osPiStartDma.
  target_link_options(${PC_EXE} PRIVATE /SUBSYSTEM:CONSOLE /FORCE)
endif()
