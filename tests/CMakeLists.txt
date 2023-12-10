project(Tests)

set(TEST_SOURCES lrtests.cpp)

add_executable(TESTCODE ${TEST_SOURCES})

set(TESTS_DATA ${CMAKE_CURRENT_SOURCE_DIR}/data)

set(TESTS_AMOUNT 7)

target_compile_definitions(TESTCODE PRIVATE TestsAmount=${TESTS_AMOUNT}
                                            TestOutputPath=\"${TESTS_DATA}/output\"
                                            TestAnswerPath=\"${TESTS_DATA}/answers\"
                                            TestDataPath=\"${TESTS_DATA}/input\"
)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(TESTCODE ${GTEST_LIBRARIES} lrLib)
target_include_directories(TESTCODE PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(TESTCODE PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(TESTCODE GTest::gtest_main)
target_include_directories(TESTCODE PRIVATE ${CMAKE_SOURCE_DIR})
