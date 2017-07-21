# Build comparison executable with Python/C API and not with lwtnn
include_directories(${PYTHON_INCLUDE_DIR})

add_executable(test_pythonc test_pythonc.cxx)

target_link_libraries(test_pythonc ${ROOT_LIBRARIES} ${PYTHON_LIBRARIES})
