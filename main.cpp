#include "predictor.h"


int main(void){
//
//    Predictor pred1;
//    pred1.ic_sim();
//    pred1.run_sim();

	std::string f = "./data/noaa_datasets/gfs.t12z.pgrb2.0p25.anl"; //"./data/noaa_datasets/gfs.t12z.mastergrb2f00"
    
    Weather_Dataset wx;
    wx.load(f);


    wx.task();




    return(0);
};

