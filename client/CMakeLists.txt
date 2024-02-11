# Add the source files to the client executable
file(GLOB_RECURSE CLIENT_SOURCES "*.cpp")

# Create the client executable
add_executable(Client ${CLIENT_SOURCES})

# Include the common directory for shared code
target_include_directories(Client PRIVATE ${CMAKE_SOURCE_DIR}/common)

# Include directories
target_include_directories(Client PRIVATE ${WEBSOCKETPP_INCLUDE_DIR})
