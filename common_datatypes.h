/*************************************************************************************************************
 *
 * Description: Contains Balua Predictor basic data types and general contants
 *
 *************************************************************************************************************/
#pragma once

#include <chrono>

#define RADIUS_OF_EARTH 6371009.0

//predictor wind data sources NOAA and Portugese Meteorogical Institute
enum data_source_type {nooa, meteo_ist};

//states of ballon simulation 
enum state_type {descending=-1,burst,ascending,landed,stop_sim};




//base simulation block that represent one data point
struct pred_block 
{
    enum state_type state;
    std::chrono::system_clock::time_point time; 
    double alt;
    double lat;
    double lon; 
    double u_wind; 
    double v_wind; 
};


