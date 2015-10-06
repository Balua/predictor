#pragma once

#include <string>
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


              
