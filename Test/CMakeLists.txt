# some test exe
file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/Test/*test.cpp")

foreach (test_source ${TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(test_filename ${test_source} NAME)
    string(REPLACE ".cpp" "" test_name ${test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${test_name} ${test_source})
endforeach ()

target_link_libraries(json-test PRIVATE nlohmann_json::nlohmann_json)