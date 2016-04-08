#pragma once

#include <string>
#include <vector>
#include "common_datatypes.h"
#include "grib_api.h"    //headers for grib


//weather class (will deal with wind models)
class Weather_Dataset
{
    private:
        //enum data_source_type data_source;
        std::string  m_filename    = "";
        grib_handle *m_grib_handle = NULL;
        FILE 		*m_file_handle = NULL;

        std::vector<unsigned int> noaa_press_level = {10, 20, 30, 50, 70, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 925, 950, 975, 1000};

        //functions
    public:
        ~Weather_Dataset();
        //get wind variables for a 4D point
        void get_wind(struct pred_block *cur_block);

        void task();

        //load dataset
        void load(std::string p_filename);
        void unload();
};
