cmake_minimum_required(VERSION 3.16)
project(session_5_task_filter LANGUAGES CXX)

add_executable(filter-tasks-test filter-tasks.cpp filter-tasks-test.cpp)
target_compile_features(filter-tasks-test PRIVATE cxx_std_20)
set_target_properties(filter-tasks-test PROPERTIES CXX_EXTENSIONS OFF)

enable_testing()
add_test(NAME owner-baseline COMMAND filter-tasks-test)
