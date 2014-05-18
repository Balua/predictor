#include "predictor.h"

int main(void){

    Predictor pred1;
    pred1.ic_sim();
    pred1.run_sim();


    //using namespace std::chrono;

    //
   
    //dsec one_day(3.14);


    //system_clock::time_point beg= system_clock::now();
    //system_clock::time_point end= beg + duration_cast<nanoseconds>(one_day) + ms;

    //dsec fuck = end - beg;
    //std::cout << fuck.count() << std::endl; 


    //time_t tt;

    //tt = system_clock::to_time_t ( today );
    //std::cout << "today is: " << ctime(&tt);

    //tt = system_clock::to_time_t ( tomorrow );
    //std::cout << "tomorrow will be: " << ctime(&tt);







    return 0;
};

