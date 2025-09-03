# some test exe
file(GLOB_RECURSE CPPS_SOURCES "${PROJECT_SOURCE_DIR}/Boost-Learn/*.cpp")

foreach (cpp_source ${CPPS_SOURCES})
    # Create a human readable name.
    get_filename_component(cpp_filename ${cpp_source} NAME)
    string(REPLACE ".cpp" "" cpp_name ${cpp_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${cpp_name} ${cpp_source})
    target_link_libraries(${cpp_name} PRIVATE 
        Boost::filesystem Boost::regex 
        Boost::container Boost::coroutine2 Boost::fiber)
endforeach ()
