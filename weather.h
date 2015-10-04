#pragma once

#include <string>
#include "common_datatypes.h"



//weather class (will deal with wind models) 
class Weather_Dataset  
{ 
    //data containers 
    public: 
        enum data_source_type data_source; 
        std::string file_name;   

        //functions 
    public: 
        //get wind variables for a 4D point 
        void get_wind(struct pred_block *cur_block); 

        //load dataset 
        void load(); 
        void unload(); 
}; 


              
