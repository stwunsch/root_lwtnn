# Define test case sources
set(TEST_CASE_LIST
    qa_data
    qa_node
    qa_tree
    qa_builder)

# Go through sources and add tests to test system
FOREACH(TEST_CASE ${TEST_CASE_LIST})
    # Set source path
    SET(TEST_CASE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_CASE}.cc)

    # Add test executables
    add_executable(${TEST_CASE} ${TEST_CASE_SOURCE})

    # Link test executables to project
    target_link_libraries(${TEST_CASE} bdt)

    # Add tests to test system
    add_test(${TEST_CASE} ${TEST_CASE})
ENDFOREACH(TEST_CASE)
