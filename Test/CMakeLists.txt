# Test executable
add_executable(
        Test_Bottin
        Test_Bottin.cpp
)

# Include headers from the main project
target_include_directories(Test_Bottin PRIVATE ${PROJECT_SOURCE_DIR})

# Link the main project library and GoogleTest
target_link_libraries(
        Test_Bottin
        tp3_bottin_lib  # Main project library
        gtest_main      # GoogleTest main library
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(Test_Bottin)