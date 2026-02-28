cmake_minimum_required(VERSION 3.15)

# Name of your project
project(DataStructureVisualizer)

# Force C++17 standard (Highly recommended for modern UI libraries)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
# =========================================================================
