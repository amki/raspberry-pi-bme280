cmake_minimum_required (VERSION 2.6)
project (Sensei)

include_directories ("BME280_driver_cmake")
add_subdirectory (BME280_driver_cmake)

add_executable(Sensei sense.cpp)
target_link_libraries(Sensei bme280)