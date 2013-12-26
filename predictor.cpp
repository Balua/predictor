#include <iostream>
#include <vector>
#include <string>
#include <math.h>


class Dataset {

    private:
        enum data_source {nooa, meteo_ist};
        unsigned int uid;
        std::vector<double> lat_nodes;
        std::vector<double> lon_nodes;
        std::vector<double> pressure_nodes;
        std::vector<double> alt_nodes;
        std::vector<double> time_nodes;
        std::vector<double> u_wind_nodes;
        std::vector<double> v_wind_nodes;


    public:
        //load dataset
        void load(unsigned int UID){
         
           uid=UID;

        }

        //get wind variables for a 4D point
        void get_wind(double time, double alt, double lat, double lon, double &u_wind, double &v_wind){


        }

        void unload(){
        

        }
};




class Predictor {
    private:
        Dataset dataset; 
        double burst_alt;
        double ascent_rate;
        double drag_coef;
        double timestep;
        enum state_type {descending=-1,burst,ascending,landed,stop_sim};

        struct pred_block {
            enum state_type state;
            double time;
            double alt;
            double lat;
            double lon; 
        };
        
        std::vector<pred_block> vec_blocks; 
       
             

    private:
        //ISA density
        double get_density(double altitude) {
    
            double temp = 0.0, pressure = 0.0;
         
            if (altitude > 25000) {
                temp = -131.21 + 0.00299 * altitude;
                pressure = 2.488*pow((temp+273.1)/216.6,-11.388);
            }
            if (altitude <=25000 && altitude > 11000) {
                temp = -56.46;
                pressure = 22.65 * exp(1.73-0.000157*altitude);
            }
            if (altitude <=11000) {
                temp = 15.04 - 0.00649 * altitude;
                pressure = 101.29 * pow((temp + 273.1)/288.08,5.256);
            }
            
            return pressure/(0.2869*(temp+273.1));
        };
        
        //calculate longitudinal model variable (alt) and set's correct simulation state 
        void set_altitude(struct pred_block *cur_block){ 
            
            //test in which state we where before            
            switch(vec_blocks.back().state){
                case descending:
                        cur_block->alt=vec_blocks.back().alt - timestep*drag_coef/sqrt(get_density(vec_blocks.back().alt));   
                        if(cur_block->alt<=0.0){
                            cur_block->state = landed; 
                            cur_block->alt=0.0; // update altitude 
                        }
                        else{
                            cur_block->state = descending; 
                        }
                        break;
                case burst:
                        cur_block->alt = vec_blocks.back().alt;
                        cur_block->state = descending; 
                        timestep  = 3.0 ;   //seconds
                        break;
                case ascending:
                        cur_block->alt = vec_blocks.back().alt + ascent_rate*timestep;
                        if(cur_block->alt>= burst_alt){
                            cur_block->state = burst; 
                        } 
                        else{
                            cur_block->state = ascending; 
                        }  
                        break;
                case landed:
                        cur_block->state = stop_sim; 
                        break;
                case stop_sim:
                        break;
            };

        };


        //calculates lateral model variables (lat and lon) 
        void set_lat_lon(struct pred_block *cur_block){


        }

        void print_block(struct pred_block cur_block){
            std::cout << cur_block.time << " -> "  <<  cur_block.state <<  " , "  <<  cur_block.alt << " , "  <<  cur_block.lat << " , "  <<  cur_block.lon << std::endl;
        }


    public:
        //set initial condiions 
        void ic_sim(){
            //simulation parameters 
            burst_alt = 30000;  //meters
            ascent_rate = 10.0;  //(meters/second)          
            drag_coef = 0.9;
            timestep  = 1.0 ;   //seconds
            
             
            //set initialization block 
            struct pred_block ic_block;
            ic_block.state=ascending;
            ic_block.time=0.0; 
            ic_block.alt=0.0;
            ic_block.lat=32.0231;
            ic_block.lon=-8.43268;
            // push it back to vector 
            vec_blocks.push_back(ic_block); 

            std::cout << "----------Balua Predictor----------" << std::endl;
            std::cout << "-----Initial Conditions Set-----" << std::endl;   
            std::cout << "burst_altitude=" << burst_alt << std::endl;
            std::cout << "ascent_rate=" << ascent_rate << std::endl;
            std::cout << "drag_coefficient=" << drag_coef << std::endl;
            std::cout << "timestep=" << timestep << std::endl;
            std::cout << "state=" << vec_blocks.back().state << std::endl;
            std::cout << "time=" << vec_blocks.back().time << std::endl;
            std::cout << "altitude=" << vec_blocks.back().alt << std::endl;
            std::cout << "latitude=" << vec_blocks.back().lat << "N"<< std::endl;
            std::cout << "longitude=" << vec_blocks.back().lon << "W"<< std::endl;
             
        };

        //set initial condiions 
        void run_sim(){

            //assign new prediction block
            struct pred_block cur_block;

            //run until sim stops
            while(vec_blocks.back().state != stop_sim){

                cur_block.time = vec_blocks.back().time + timestep; //set time on current block
            
                set_altitude(&cur_block);   //set altitude on current block
                set_lat_lon(&cur_block);    // set lat lon on current block 
                print_block(vec_blocks.back());  //print last block
                
                vec_blocks.push_back(cur_block);    //add current block to vector of blocks
            }

            std::cout << "end of simulation"<< std::endl;
            std::cout << "#blocks=" << vec_blocks.size() << std::endl;

        };


};














int main(void){

    Predictor pred1;
    pred1.ic_sim();
    pred1.run_sim();


    return 0;
};




