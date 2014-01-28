#include "predictor.h"


class Weather_Dataset {
    //data containers
    private:
        enum data_source_type data_source;
        std::string file_name;  

    //functions
    public:
        //load dataset
        void load(std::string in_file){
        }

        //get wind variables for a 4D point
        void get_wind(struct pred_block *cur_block){
            //temporary bypass values
            cur_block->v_wind = 1.0;
            cur_block->u_wind = 1.0;
        }

        
        void unload(){ }
};

class Predictor {
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
        void read_predition_xml(double *burst_alt,double *ascent_rate,double *drag_coef,double *timestep,struct pred_block *ic_block){

        };

        //ISA density could be implemented using the real atmosphere data
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
            double ddlat;
            double ddlon; 
            double theta, r;
           

            //get dd angles 
            // See the differentiation section of
            // http://en.wikipedia.org/wiki/Spherical_coordinate_system
            theta = 2.0 * M_PI * (90.0 - cur_block->lat) / 360.0;
            r = RADIUS_OF_EARTH + cur_block->alt;
            // d/dv = d/dlat = -d/dtheta
            ddlat = (2.0 * M_PI) * r / 360.0;
            // d/du = d/dlong = d/dphi
            ddlon = (2.0 * M_PI) * r * sin(theta) / 360.0;


            cur_block->lat = vec_blocks.back().lat  + (vec_blocks.back().v_wind*timestep)/ddlat; 
            cur_block->lon = vec_blocks.back().lon  + (vec_blocks.back().u_wind*timestep)/ddlon; 

        }


        //simple easy print function to output in the right format
        void print_block(struct pred_block cur_block){
            std::cout.precision(15);
            std::cout << cur_block.time << ", "  <<  cur_block.state <<  ", "  <<  cur_block.alt << ", "  <<  cur_block.lat << ", "  <<  cur_block.lon << std::endl;
        }



    public:
        //set hard coded initial condiions 
        void hard_ic_sim(){
            struct pred_block ic_block; //temp block
            
            //simulation parameters 
            burst_alt = 30000;  //meters
            ascent_rate = 10.0;  //(meters/second)          
            drag_coef = 0.9;
            timestep  = 1.0 ;   //seconds
                         
            //set initialization block 
            ic_block.state=ascending;
            ic_block.time=0.0; 
            ic_block.alt=0.0;
            ic_block.lat=32.0231;
            ic_block.lon=-8.43268;
           
            dataset.get_wind(&ic_block); //fill wind info
            
            vec_blocks.push_back(ic_block); //push it back

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
        void ic_sim(){
            struct pred_block ic_block; //temp block
        
            read_predition_xml(&burst_alt,&ascent_rate,&drag_coef,&timestep,&ic_block);//fill it with xml data 

            dataset.get_wind(&ic_block); //fill it with wind conditions
            
            vec_blocks.push_back(ic_block);//push it back to vector 

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
        }

        //run sim 
        void run_sim(){
            //assign new prediction block
            struct pred_block cur_block;

            //run until sim stops
            while(vec_blocks.back().state != stop_sim){

                cur_block.time = vec_blocks.back().time + timestep; //set time on current block
            
                set_altitude(&cur_block);   //set altitude on current block
                set_lat_lon(&cur_block);    // set lat lon on current block 
                dataset.get_wind(&cur_block); //get wind on current block
                
                vec_blocks.push_back(cur_block);    //add current block to vector of blocks
                //print_block(vec_blocks.back());  //print last block
            }

            std::cout << "end of simulation"<< std::endl;
            std::cout << "#blocks=" << vec_blocks.size() << std::endl;
        };

};



int main(void){

    Predictor pred1;
    pred1.hard_ic_sim();
    pred1.run_sim();

    return 0;
};




