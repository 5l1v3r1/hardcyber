# Hardcyber - PC-64k-Intro by Team210 at Deadline 2k19
# Copyright (C) 2019 Alexander Kraus <nr4@z10.info>
# Copyright (C) 2019 DaDummy <c.anselm@paindevs.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.12)

project(hardcyber)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configurables
set(MINIFY_64K ON CACHE BOOL "Compress the executable using kkrunchy7 (disables hybrid graphics dGPU exports)")

# Tell CMake to run qt tools when necessary (For Qt5 GUIs among other things)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the right python interpreter version
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(Qt5 COMPONENTS Widgets)

set(COMMON_HEADERS config_types.h config.h glext.h engine/platform.h engine/renderer.h engine/shader.h engine/loader.h engine/orchestrator.h)
set(SFX_SHADER sfx.frag)
set(SHADER_FILES
        gfx/load.frag
        gfx/ocean.frag
        gfx/logo210.frag
        gfx/graffiti.frag
        gfx/starsky.frag
        gfx/text.frag
        gfx/post.frag
        gfx/deadline.frag
        gfx/hydrant.frag
        gfx/watercubes.frag
        gfx/glitchcity.frag
        gfx/greetings.frag
        gfx/fractal.frag
        gfx/voronoinet.frag
        gfx/volclouds.frag
)
set(SHADER_SYMBOLS_PATH gfx/symbols/)
set(STRINGS_FILE font/strings.txt)


#################################
# Demo executable
#################################

set(DEMO_HEADERS common.h draw.h scenes.h pal_linux.h pal_win32.h sequence.h)

set(SFX_HEADER sfx.h)
add_custom_target(minify_sfx
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/minification/
    COMMAND "${Python3_EXECUTABLE}" shader-compressor.py -o "${PROJECT_SOURCE_DIR}/${SFX_HEADER}" "${PROJECT_SOURCE_DIR}/${SFX_SHADER}" --no-minification
    SOURCES minification/shader-compressor.py ${SFX_SHADER}
    BYPRODUCTS ${SFX_HEADER}
)

set(SHADER_HEADER shaders.gen.h)
add_custom_target(minify_shaders
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
    COMMAND "${Python3_EXECUTABLE}" minification/symbolize.py ${SHADER_FILES} -s ${SHADER_SYMBOLS_PATH} -o ${SHADER_HEADER}
    SOURCES minification/symbolize.py minification/Rule.py minification/Token.py minification/GLSLLexer130.py minification/Compressor.py ${SHADER_FILES}
    BYPRODUCTS ${SHADER_HEADER}
)

set(FONT_HEADER font/font.h)
add_custom_target(texture_packing
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/font/
    COMMAND "${Python3_EXECUTABLE}" pack.py
    SOURCES font/pack.py ${STRINGS_FILE}
    BYPRODUCTS ${FONT_HEADER}
)

add_executable(hardcyber
    ls.c
    ${COMMON_HEADERS}
    ${DEMO_HEADERS}
    ${SFX_SHADER}
    ${STRINGS_FILE}
    ${FRAGMENT_SHADERS}
    ${SHADER_FILES}
)
add_dependencies(hardcyber minify_sfx)
add_dependencies(hardcyber minify_shaders)
add_dependencies(hardcyber texture_packing)

target_include_directories(hardcyber SYSTEM PRIVATE "${OPENGL_INCLUDE_DIR}")

target_link_libraries(hardcyber OpenGL::GL)

if(MSVC)
    set_target_properties(hardcyber PROPERTIES LINK_FLAGS "/ENTRY:demo /SUBSYSTEM:Windows /OPT:REF /OPT:ICF=10 /VERBOSE /NODEFAULTLIB")
    target_compile_options(hardcyber PRIVATE /Gy /O1 /fp:fast /GR- /GS- /MT )

    if (MSVC_VERSION GREATER_EQUAL 1910) # VS2017
        target_link_libraries(hardcyber User32.lib Winmm.lib ucrt.lib msvcrt.lib vcruntime.lib Vfw32.lib)
    else()
        # for debug output: User32.lib ucrt.lib msvcrt.lib
        target_link_libraries(hardcyber User32.lib Winmm.lib ucrt.lib msvcrt.lib)
    endif()

    if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND MINIFY_64K AND (MSVC_IDE OR CMAKE_BUILD_TYPE STREQUAL "Release"))
        target_compile_definitions(hardcyber PRIVATE "$<$<CONFIG:RELEASE>:MINIFY_64K>")
        find_program(KKRUNCHY_K7 kkrunchy_k7.exe HINTS "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/bin")
        add_custom_command(TARGET hardcyber COMMAND "$<$<CONFIG:RELEASE>:${KKRUNCHY_K7}>" ARGS --best "$<TARGET_FILE:hardcyber>" --out "$<TARGET_FILE_DIR:hardcyber>/hardcyberc.exe" POST_BUILD)
    endif()
else()
    find_package(SDL2 REQUIRED)
    target_include_directories(hardcyber SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(hardcyber ${SDL2_LIBRARIES})
endif()


#################################
# Studio executable
#################################

add_executable(studio
    studio/studio.cpp
    studio/mainwindow.cpp
    studio/mainwindow.ui
    studio/demoapi.cpp
    studio/renderviewwidget.cpp
    ${COMMON_HEADERS}
)

add_dependencies(studio minify_sfx)
add_dependencies(studio minify_shaders)
add_dependencies(studio texture_packing)

target_link_libraries(studio Qt5::Widgets)
target_link_libraries(studio OpenGL::GL)
