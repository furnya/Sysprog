cmake_minimum_required(VERSION 3.5.1)
project(scheduler)

set(CMAKE_CXX_STANDARD 11)

# Enabling this will speed things up.
# add_definitions(-D NO_FANCY)

# New Source files have to be added to that list
set(SOURCE_FILES
        Tasks/cJSON.c
        Tasks/Task.c
        Tasks/TaskPool.c
        Scheduling/Scheduling.c
        Scheduling/Monkey.c
        Scheduling/FCFS.c
        Scheduling/SJN.c
        Scheduling/RR.c
        Scheduling/MLF.c
        main.c)

add_executable(scheduler ${SOURCE_FILES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(scheduler Threads::Threads)
target_link_libraries(scheduler m)
