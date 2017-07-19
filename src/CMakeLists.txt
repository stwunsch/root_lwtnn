# Add all sources in this folder to the project
file(GLOB SOURCES "*.cxx")

# Make an executable
add_executable(root_lwtnn ${SOURCES})

# Link libraries to the executable
target_link_libraries(root_lwtnn ${ROOT_LIBRARIES} ${Boost_LIBRARIES})
