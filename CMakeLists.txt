cmake_minimum_required(VERSION 3.4)

project(tgaProjekt)



add_executable(tgaProjekt main.c header.h fce.h)

target_link_libraries(tgaProjekt m)