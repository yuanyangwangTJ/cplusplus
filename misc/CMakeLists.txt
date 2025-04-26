# some test exe
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/misc/*test.cpp")

foreach (test_source ${TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${test_name} ${test_source})
endforeach ()

# address sanitizer
set(ASAN_TARGET sanitizer-test)
target_compile_options(${ASAN_TARGET} PRIVATE -fsanitize=address)
target_link_options(${ASAN_TARGET} PRIVATE -fsanitize=address)
