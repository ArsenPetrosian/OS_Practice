cmake_minimum_required(VERSION 3.22)

project(copy)

file(GLOB copy_files "./*.cpp")
add_executable(copy ${copy_files})