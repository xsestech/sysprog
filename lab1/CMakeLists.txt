cmake_minimum_required(VERSION 3.27)
project(sysprog C)

set(CMAKE_C_STANDARD 99)
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake")
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
option(LIBCPP "Build with libc++" FALSE)
option(ENABLE_ASAN "Enable address sanitizer" FALSE)
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
option(SIMPLE_BUILD "Build the project as minimally as possible" FALSE)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
option(DEBUG_LOGGING "Enabling debug logging" FALSE)
#option(BUILD_DOC "Build documentation" FALSE)
add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

target_compile_features(project_options INTERFACE c_std_23)


if (ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)

endif ()

target_compile_options(project_warnings
        INTERFACE
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
        #        -Wconversion
        #        -Wsign-conversion
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2)

# some GCC specific warnings. These flags are added only if the used compiler is GCC.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(project_warnings
            INTERFACE
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wlogical-op
            -Wuseless-cast
    )
    target_link_libraries(project_options INTERFACE stdc++fs)
endif ()

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(project_options INTERFACE -fcolor-diagnostics)
    endif ()
endif ()

if (${DEBUG_LOGGING})
    target_compile_options(project_options INTERFACE -g)
    target_link_libraries(project_options INTERFACE -g)
endif ()

find_program(CCACHE ccache)
if (CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif ()

if (ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if (CPPCHECK)
        set(CMAKE_CXX_CPPCHECK
                ${CPPCHECK}
                --suppress=syntaxError
                --suppress=missingIncludeSystem
                --suppress=unusedFunction
                --check-level=exhaustive
                --enable=all
                --inconclusive)
        set(CMAKE_C_CPPCHECK
                ${CPPCHECK}
                --suppress=syntaxError
                --suppress=unusedFunction
                --suppress=missingIncludeSystem
                --check-level=exhaustive
                --enable=all
                --inconclusive)
    else ()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif ()
endif ()

if (ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if (CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    else ()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif ()
endif ()

message("#########################################")
message("\t \t Summary")
message("#########################################")
message("Build type:       \t ${CMAKE_BUILD_TYPE}")
message("Install prefix:   \t ${CMAKE_INSTALL_PREFIX}")
message("Testing enabled:  \t ${ENABLE_TESTING}")
message("Clang-tidy:       \t ${ENABLE_CLANG_TIDY}")
message("Cppcheck:         \t ${ENABLE_CPPCHECK}")
message("Compiler:         \t ${CMAKE_CXX_COMPILER_ID}")
message("Sanizizers:       \t ${ENABLE_ASAN}")
message("Build libcpp:     \t ${LIBCPP}")
message("CCache executable:\t ${CCACHE}")
message("------------------------------------------")
message("Version:          \t ${PROJECT_VERSION}")

add_subdirectory(libicli)
add_subdirectory(task1)


#target_link_libraries(libicli PUBLIC liberrors project_options)
target_link_libraries(task1 PUBLIC libicli project_options
        project_warnings)

if (APPLE)
    find_program(DSYMUTIL_PROGRAM dsymutil)
    add_custom_command(TARGET ${task_name} POST_BUILD
            COMMAND dsymutil $<TARGET_FILE:${task_name}>
            COMMENT Generate dSYM files for ${task_name}
            OUTPUT $
    )
endif ()
