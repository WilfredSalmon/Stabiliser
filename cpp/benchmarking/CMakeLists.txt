add_subdirectory(stim)

find_package( benchmark REQUIRED )

add_executable( 
	benchmark
	stabiliser_from_statevector_benchmark.cpp
)

target_link_libraries(
	benchmark
	PRIVATE
		benchmark::benchmark
		benchmark::benchmark_main
		stim
		stabiliser_state 
)
