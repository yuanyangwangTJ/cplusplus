# Json
add_executable(json-wheel
	jsonparse.hpp 
	main.cpp
	print.h
)

target_compile_features(json-wheel 
	PRIVATE cxx_std_20
)
set(ASAN_TARGET json-wheel)
target_compile_options(${ASAN_TARGET} PRIVATE -fsanitize=address)
target_link_options(${ASAN_TARGET} PRIVATE -fsanitize=address)
