# thread pool
add_executable(thread-pool-test 
	threadpool.h 
	example.cpp
)

target_compile_features(thread-pool-test 
	PRIVATE cxx_std_20)
