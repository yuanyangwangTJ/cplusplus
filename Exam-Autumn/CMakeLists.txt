# some exam exe
file(GLOB_RECURSE EXAM_SOURCES "${PROJECT_SOURCE_DIR}/Exam-Autumn/*/**.cpp")

foreach (exam_source ${EXAM_SOURCES})
    # Create a human readable name.
    get_filename_component(exam_filename ${exam_source} NAME)
    string(REPLACE ".cpp" "" exam_name ${exam_filename})

    # Add the exam target separately and as part of "make check-exams".
    add_executable(${exam_name} ${exam_source})
endforeach ()
