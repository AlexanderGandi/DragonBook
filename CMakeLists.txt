cmake_minimum_required(VERSION 4.3)
project(DragonBook
        VERSION 0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

add_library(core
        src/Token.cpp
        src/Token.h
        src/Position.cpp
        src/Position.h
        src/Lexer.cpp
        src/Lexer.h
        src/ASTNode.cpp
        src/ASTNode.h
        src/Parser.cpp
        src/Parser.h
        src/CodeViewer.cpp
        src/CodeViewer.h
        src/Environment.cpp
        src/Environment.h
        src/LexerUtils.cpp
        src/LexerUtils.h
        src/ExceptionReporter.cpp
        src/ExceptionReporter.h
        src/CodeManager.cpp
        src/CodeManager.h
)

target_include_directories(core
        PUBLIC
        src
        external/include
)

add_executable(DragonBook src/main.cpp)
target_link_libraries(DragonBook PRIVATE core)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
FetchContent_MakeAvailable(googletest)
add_subdirectory(tests)


