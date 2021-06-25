# -----------------------------------------------------------------------------
# TEST 1 (This one uses the dataset from file)
# -----------------------------------------------------------------------------
# Add the test.
add_executable(test_1_executable
    test_1.cpp
)
# Liking for the test.
target_link_libraries(
    test_1_executable
    antlr4_static
    expar
)
add_test(test_1 test_1_executable)
