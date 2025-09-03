add_executable(AddressSanitizer-test AddressSanitizer-test.cpp)

target_compile_options(AddressSanitizer-test PRIVATE -fsanitize=address -g -O1)
target_link_options(AddressSanitizer-test PRIVATE -fsanitize=address)
