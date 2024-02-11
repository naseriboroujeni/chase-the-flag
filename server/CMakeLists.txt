# Add the source files to the server executable
file(GLOB_RECURSE SERVER_SOURCES "*.cpp")

# Create the server executable
add_executable(Server ${SERVER_SOURCES})

# Include directories
target_include_directories(Server PRIVATE ${WEBSOCKETPP_INCLUDE_DIR})
