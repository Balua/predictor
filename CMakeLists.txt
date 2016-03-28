########################################################################
#
# Balua predictor cmake file.
#
########################################################################


cmake_minimum_required(VERSION 2.8)

project(predictor)

include_directories(dependencies/pugixml/src/)

link_libraries(grib_api)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(SOURCE_FILES
    dependencies/pugixml/src/pugiconfig.hpp
    dependencies/pugixml/src/pugixml.cpp
    dependencies/pugixml/src/pugixml.hpp
    common_datatypes.h
    main.cpp
    predictor.cpp
    predictor.h
    weather.cpp
    weather.h)


add_executable(predictor ${SOURCE_FILES})
