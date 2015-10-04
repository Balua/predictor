#pragma once

#include <vector>
#include "weather.h"
#include "common_datatypes.h"




//class of prediction
class Predictor
{
    //data containers 
    private:
        Weather_Dataset dataset; 
        double burst_alt;
        double ascent_rate;
        double drag_coef;
        double timestep;
        std::vector<pred_block> vec_blocks; 


        //functions
    private:
        //read prediction xml 
        void read_predition_xml(double *burst_alt,double *ascent_rate,double *drag_coef,double *timestep,struct pred_block *ic_block);

        //ISA density could be implemented using the real atmosphere data
        double get_density(double altitude); 

        //calculate longitudinal model variable (alt) and set's correct simulation state 
        void set_altitude(struct pred_block *cur_block);

        //calculates lateral model variables (lat and lon) 
        void set_lat_lon(struct pred_block *cur_block);

        //
        void fligth_statistics();

        //simple easy print function to output in the right format
        void print_block(struct pred_block cur_block);


    public:
        //set initial condiions 
        void ic_sim();

        //run sim 
        void run_sim();

};


