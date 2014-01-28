#include <iostream>
#include <vector>
#include <string>
#include <math.h>



#define RADIUS_OF_EARTH 6371009.0

enum data_source_type {nooa, meteo_ist};

enum state_type {descending=-1,burst,ascending,landed,stop_sim};

struct pred_block {
    enum state_type state;
    double time;
    double alt;
    double lat;
    double lon; 
    double u_wind; 
    double v_wind; 
};





