# Set minimum cmake version
cmake_minimum_required(VERSION 3.4.3)

# Enable test cases
#enable_testing()

# Setup project
project(root_lwtnn CXX)

# Find ROOT
# NOTE: The ROOT environment has to be set up, so that the
# environment variable ROOTSYS is populated.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO)

# Find Boost
find_package(Boost REQUIRED COMPONENTS program_options)

# Find lwtnn
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(lwtnn REQUIRED)

# Add include directories
include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${LWTNN_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(src)
#add_subdirectory(test)
