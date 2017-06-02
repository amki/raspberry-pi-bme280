cmake_minimum_required (VERSION 2.8)
project (Sensei)

add_compile_options(-std=c++11)

find_package(Threads REQUIRED)

FIND_PACKAGE( Boost 1.55 COMPONENTS system date_time regex REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

find_library(CPPNET cppnetlib-client-connections cppnetlib-server-parsers cppnetlib-uri)

add_executable(Sensei sense.cpp)

include_directories ("BME280_driver_cmake")
add_subdirectory (BME280_driver_cmake)


if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(PUBLIC Sensei "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(Sensei "${CMAKE_THREAD_LIBS_INIT}")
endif()
target_link_libraries(Sensei bme280 ${CPPNET} ${Boost_LIBRARIES})
