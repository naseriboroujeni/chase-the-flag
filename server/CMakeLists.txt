# Add the source files to the server executable
file(GLOB_RECURSE SERVER_SOURCES "*.cpp")

# Create the server executable
add_executable(Server ${SERVER_SOURCES})

# Include the common directory for shared code
target_include_directories(Server PRIVATE ${CMAKE_SOURCE_DIR}/common)

# Include directories
target_include_directories(Server PRIVATE ${WEBSOCKETPP_INCLUDE_DIR})
