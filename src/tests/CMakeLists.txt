find_package( Catch2 3 REQUIRED )

set( SOURCE_FILES
  f2_helper_test.cpp
  stabiliser_state_test.cpp
  stabiliser_state_from_statevector_test.cpp
  pauli_test.cpp
  check_matrix_test.cpp
  test_util.cpp
)

add_executable( tests ${SOURCE_FILES} )

target_include_directories( tests PUBLIC
  "${PROJECT_SOURCE_DIR}/src/stabiliser_state"
  "${PROJECT_SOURCE_DIR}/src/util"
  "${PROJECT_SOURCE_DIR}/src/pauli"
  "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(tests PRIVATE 
  Catch2::Catch2WithMain 
  stabiliser_state
  pauli
)

include(CTest)
include(Catch)
catch_discover_tests(tests)