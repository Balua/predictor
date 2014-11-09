#include <iostream>
#include <ctime>
#include <ratio>
#include <vector>
#include <string>
#include <math.h>
#include <chrono>
#include <locale>
#include "pugixml.hpp"      //headers for xml 
#include "pugiconfig.hpp"   
#include "grib_api.h"       //headers for grib





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


