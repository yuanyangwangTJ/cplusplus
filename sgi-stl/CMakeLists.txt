file(GLOB SGI_STL_SRCS "./src/*")

set(SGI_STL_LIB_NAME sgi-stl)
add_library(${SGI_STL_LIB_NAME} ${SGI_STL_SRCS})
set_target_properties(${SGI_STL_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

#target_include_directories(${SGI_STL_LIB_NAME} PUBLIC
#    "./src"
#)
include_directories(src/)
