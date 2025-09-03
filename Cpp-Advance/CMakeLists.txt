file(GLOB_RECURSE CPPS_SOURCES "${PROJECT_SOURCE_DIR}/Cpp-Advance/*.cpp")

foreach (cpp_source ${CPPS_SOURCES})
    # Create a human readable name.
    get_filename_component(cpp_filename ${cpp_source} NAME)
    string(REPLACE ".cpp" "" cpp_name ${cpp_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${cpp_name} ${cpp_source})
endforeach ()
