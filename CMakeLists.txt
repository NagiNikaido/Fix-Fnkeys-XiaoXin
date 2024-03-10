cmake_minimum_required(VERSION 3.0)

project(fix-fnkeys-xiaoxin)

add_executable(fix-fnkeys-xiaoxin fix-fnkeys-xiaoxin.c)
target_compile_options(fix-fnkeys-xiaoxin PRIVATE -Wall -Wextra)

install(TARGETS fix-fnkeys-xiaoxin RUNTIME DESTINATION bin)

