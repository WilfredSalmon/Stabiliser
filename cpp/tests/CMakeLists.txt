find_package( Catch2 3 REQUIRED )

add_executable( tests
    f2_helper_test.cpp
    stabiliser_state_test.cpp
    stabiliser_state_from_statevector_test.cpp
    pauli_test.cpp
)

target_link_libraries( tests PRIVATE Catch2::Catch2WithMain stabiliser_state )

include(CTest)
include(Catch)
catch_discover_tests(tests)
