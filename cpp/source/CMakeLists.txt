# Add interal source files and non exported headers here
add_library( stabiliser_state
    stabiliser_state.cpp
    stabiliser_state_from_statevector.cpp
    pauli.cpp
)

# Automatically include all public headers from the include folder
target_include_directories( stabiliser_state
    PUBLIC
        $<INSTALL_INTERFACE:../include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
