set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

# Add interal source files and non exported headers here
add_library( ${LIBRARY_TARGET_NAME}
    stabiliser_state.cpp
    stabiliser_state_from_statevector.cpp
    pauli.cpp
)

# Automatically include all public headers from the include folder
target_include_directories( ${LIBRARY_TARGET_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/../include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
