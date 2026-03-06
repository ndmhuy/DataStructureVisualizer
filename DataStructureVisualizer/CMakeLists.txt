cmake_minimum_required(VERSION 3.15)

# Name of your project
project(DataStructureVisualizer)

# Force C++17 standard (Highly recommended for modern UI libraries)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# This option is for generating compile_commands.json, which is useful for code editors and IDEs to understand your project structure and provide features like code navigation and auto-completion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Recursively find all .cpp implementation files inside the src/ folder
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the final executable file using your source files
# (This fulfills the requirement to output an .exe for your release )
add_executable(${PROJECT_NAME} ${SOURCES})

# Tell the compiler where to find your .h header files
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# =========================================================================
# TODO FOR LATER: 
# When your team finalizes Raylib or SFML, you will use `find_package`
# and `target_link_libraries` right here to connect the graphics!
include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.2
)

FetchContent_MakeAvailable(SFML)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    SFML::Network
)
# =========================================================================
