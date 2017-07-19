# Set minimum cmake version
cmake_minimum_required(VERSION 3.4.3)

# Enable test cases
#enable_testing()

# Setup project
project(root_lwtnn CXX)

# Find ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO)

# Find Boost
find_package(Boost REQUIRED COMPONENTS program_options)

# Add include dir
include_directories(include)

# Add subdirectories
add_subdirectory(src)
#add_subdirectory(test)
