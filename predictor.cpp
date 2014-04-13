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

            std::string source="./data/predictions/example.xml";
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(source.c_str());
   
            if(not result == 1){
                std::cout << "Load result: " << result.description() << std::endl;
                exit(-1);
            } 
            for (pugi::xml_node tool = doc.child("prediction"); tool; tool = tool.next_sibling("prediction"))
            {
                *burst_alt = tool.child("simulation_settings").child("burst_altitude").text().as_double();
                *ascent_rate= tool.child("simulation_settings").child("ascent_rate").text().as_double();
                *timestep= tool.child("simulation_settings").child("timestep").text().as_double();
                *drag_coef= tool.child("simulation_settings").child("drag_coeficient").text().as_double();
                
                
                ic_block->state = static_cast<state_type>(tool.child("initial_conditions").child("state").text().as_int());
                ic_block->alt = tool.child("initial_conditions").child("altitude").text().as_double();
                ic_block->lon = tool.child("initial_conditions").child("longitude").text().as_double();
                ic_block->lat = tool.child("initial_conditions").child("latitude").text().as_double();

                //std::cout <<  std::put_time(std::localtime(&now_c), "%F %T") << std::endl;
                //std::cout << tool.child("initial_conditions").child("gmt_lauch_time").text().get() << std::endl;
                 

            }
    
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

                //cur_block.time = vec_blocks.back().time + std::chrono::seconds(timestep); //set time on current block
            
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

//    Predictor pred1;
//    pred1.ic_sim();
//    pred1.run_sim();
//
//    std::time_t time_t0;
//    std::chrono::system_clock::time_point t0;
//
//
//
//    t0 = std::chrono::system_clock::now();
//    time_t0 = std::chrono::system_clock::to_time_t(t0); 
//   
//    //std::time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); 
//    //std::cout << std::strftime(std::gmtime(&time_t0), "%Y/%m/%d %T")  << std::endl;
//    char mbstr[256];
//    if (std::strftime(mbstr, sizeof(mbstr), "%Y/%m/%d %T", std::localtime(&time_t0))) {
//        std::cout << mbstr << '\n';
//    }
//

    using namespace std::chrono;

    duration<int,std::ratio<60*60*24> > one_day (1);

    system_clock::time_point today = system_clock::now();
    system_clock::time_point tomorrow = today + one_day;

    time_t tt;

    tt = system_clock::to_time_t ( today );
    std::cout << "today is: " << ctime(&tt);

    tt = system_clock::to_time_t ( tomorrow );
    std::cout << "tomorrow will be: " << ctime(&tt);



    return 0;
};



