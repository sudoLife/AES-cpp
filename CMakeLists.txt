cmake_minimum_required(VERSION 3.15)

project(aes CXX)

set(
    SRCS 
    src/main.cpp 
    src/AES.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# add_executable(test src/test.cpp src/AES.cpp)
# target_include_directories(test PUBLIC include)