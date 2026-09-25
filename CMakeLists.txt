cmake_minimum_required(VERSION 4.3)
project(DragonBook
        VERSION 0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

add_library(core
        Token.cpp
        Token.h
        Position.cpp
        Position.h
        Lexer.cpp
        Lexer.h
        ASTNode.cpp
        ASTNode.h
        Parser.cpp
        Parser.h
        CodeViewer.cpp
        CodeViewer.h
        Environment.cpp
        Environment.h
        LexerUtils.cpp
        LexerUtils.h
        ExceptionReporter.cpp
        ExceptionReporter.h
        CodeManager.cpp
        CodeManager.h
)

target_include_directories(core
        PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/external/include/
)

add_executable(DragonBook main.cpp)
target_link_libraries(DragonBook PRIVATE core)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
FetchContent_MakeAvailable(googletest)
add_subdirectory(tests)


