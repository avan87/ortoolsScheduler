cmake_minimum_required(VERSION 3.5)
project(orToolsScheduler)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp  Assignment.cpp Assignment.h Assignments.cpp Assignments.h OrtoolsScheduler.cpp OrtoolsScheduler.h  server/scheduler_server.cpp
      proto/scheduler.grpc.pb.h proto/scheduler.pb.h proto/scheduler.pb.cc proto/scheduler.grpc.pb.cc utils.h)

include_directories(/home/metis/or-tools_Ubuntu-16.04-64bit_v4.4.3842/include)
include_directories(/usr/local/include)


link_directories(/usr/local/lib)
link_directories(/home/metis/or-tools_Ubuntu-16.04-64bit_v4.4.3842/lib)

add_executable(orToolsScheduler ${SOURCE_FILES})

target_link_libraries(orToolsScheduler ortools grpc++ grpc++_reflection protobuf pthread  dl)
