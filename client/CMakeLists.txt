# Add the source files to the client executable
file(GLOB_RECURSE CLIENT_SOURCES "*.cpp")

# Create the client executable
add_executable(Client ${CLIENT_SOURCES})

# Include directories
target_include_directories(Client PRIVATE
   ${CMAKE_SOURCE_DIR}/common
   ${CMAKE_SOURCE_DIR}/client/include
   ${WEBSOCKETPP_INCLUDE_DIR}
)
