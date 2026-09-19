add_executable(LexerTest
        LexerTest.cpp
)

target_link_libraries(LexerTest
        PRIVATE
        core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(LexerTest)