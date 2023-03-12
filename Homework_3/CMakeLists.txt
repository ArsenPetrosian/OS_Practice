cmake_minimum_required(VERSION 3.22)

project(pipe)

file(GLOB pipe_files "./*.cpp")
add_executable(pipe ${pipe_files})