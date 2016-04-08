########################################################################
#
# Balua predictor cmake file.
#
########################################################################


cmake_minimum_required(VERSION 3.5)

project(predictor)

#add_subdirectory(./dependencies/grib_api/ ./dependencies/grib_api/build)

include_directories(dependencies/pugixml/src/)
include_directories(dependencies/grib_api/src/)

link_directories(dependencies/grib_api/build/lib)
link_libraries(grib_api)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SOURCE_FILES
    dependencies/pugixml/src/pugixml.cpp
    main.cpp
    predictor.cpp
    weather.cpp
    )


add_executable(predictor ${SOURCE_FILES})
