#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <chrono>
#include <ctime>
#include <locale>
#include "pugixml.hpp"
#include "pugiconfig.hpp"



#define RADIUS_OF_EARTH 6371009.0

enum data_source_type {nooa, meteo_ist};

enum state_type {descending=-1,burst,ascending,landed,stop_sim};

struct pred_block {
    enum state_type state;
    std::string time; 
    double alt;
    double lat;
    double lon; 
    double u_wind; 
    double v_wind; 
};


