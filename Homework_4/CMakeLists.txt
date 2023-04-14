cmake_minimum_required(VERSION 3.22)

project(producer_consumer)

file(GLOB source_files "./*.c")
add_executable(producer_consumer ${source_files})

file(GLOB queue_files "./queue/*.c")
add_library(queue ${queue_files})
target_include_directories(queue PRIVATE "./include")