#include <iostream>
#include <vector>
#include <string>
#include <math.h>


class noaa_dataset {

    private:


};



class predictor {
    private:
        double burst_alt;
        double ascent_rate;
        double drag_coef;
        double timestep;
        enum state_type {descending=-1,burst,ascending,landed,stop_sim};

        std::vector<int> state;
        std::vector<double> time;
        std::vector<double> alt;
        std::vector<double> lat;
        std::vector<double> lon;
       

    public:
        
        //set initial condiions 
        void set_initial_conditions(){
            burst_alt = 30000;  //meters
            ascent_rate = 1.0;  //(meters/second)          
            drag_coef = 0.01;
            timestep  = 1.0 ;   //seconds
            state.push_back(ascending);
            time.push_back(0.0);
            alt.push_back(0.0);
            lat.push_back(32.0231);
            lon.push_back(-8.43268);
            
            std::cout << "-----Initial Conditions Set-----" << std::endl;   
            std::cout << "burst_altitude=" << burst_alt << std::endl;
            std::cout << "ascent_rate=" << ascent_rate << std::endl;
            std::cout << "drag_coefficient=" << drag_coef << std::endl;
            std::cout << "timestep=" << timestep << std::endl;
            std::cout << "state=" << state.back() << std::endl;
            std::cout << "time=" << time.back() << std::endl;
            std::cout << "altitude=" << alt.back()  << std::endl;
            std::cout << "latitude=" << lat.back()  << "N"<< std::endl;
            std::cout << "longitude=" << lon.back() << "W"<< std::endl;
             
        };
        
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
        
        //calculate next altitude step and correct state 
        void set_altitude(){
            //test in which state we where before            
            switch(state.back()){
                case descending:
                        alt.push_back(alt.back() - timestep*drag_coef/sqrt(get_density(alt.back())));   
                        if(alt.back()<=0.0){
                            state.push_back(landed);
                        }
                        else{
                            state.push_back(descending);
                        }
                        break;
                case burst:
                        alt.push_back(alt.back());
                        state.push_back(descending); //change of state   
                        timestep  = 3.0 ;   //seconds
                        break;
                case ascending:
                        alt.push_back(alt.back()+ ascent_rate*timestep);
                        if(alt.back()>= burst_alt){
                            state.push_back(burst);  
                        } 
                        else{
                            state.push_back(ascending); 
                        }  
                        break;
                case landed:
                        alt.push_back(0.00);
                        state.push_back(stop_sim);  
                        std::cout << "end of simulation"<< std::endl;
                        break;
            };
            std::cout << state.back() << "  ->   "  << alt.back() << std::endl;
        };


        void run_sim(){
            while(state.back()!= stop_sim){
               set_altitude();
            }
        };

};







int main(void){

    predictor pred1;


    std::cout << "----------Balua Predictor----------" << std::endl;
    pred1.set_initial_conditions();
    pred1.run_sim();


    return 0;

};




