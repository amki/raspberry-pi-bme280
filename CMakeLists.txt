cmake_minimum_required (VERSION 2.8)
project (Sensei)

add_compile_options(-std=c++11)

set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/include/postgresql/")

find_package(Threads REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(Sensei sense.cpp main.cpp sense.cpp sense.h pghandler.hpp pghandler.cpp)

include_directories ("BME280_driver_cmake")
add_subdirectory (BME280_driver_cmake)

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(PUBLIC Sensei "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(Sensei "${CMAKE_THREAD_LIBS_INIT}")
endif()

set(PostgreSQL_LIBRARIES "pqxx")
target_link_libraries(Sensei bme280 ${PostgreSQL_LIBRARIES})
