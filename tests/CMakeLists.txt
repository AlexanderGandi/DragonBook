add_executable(CompilerTest
        LexerTest.cpp
        ParserTest.cpp
        ReporterTest.cpp
)

target_link_libraries(CompilerTest
        PRIVATE
        core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(CompilerTest)