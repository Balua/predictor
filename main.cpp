#include "predictor.h"


int main(void){
//
//    Predictor pred1;
//    pred1.ic_sim();
//    pred1.run_sim();


    
    Weather_Dataset wx;
    wx.load("./data/noaa_datasets/gfs.t12z.mastergrb2f00");


    wx.task();




    return(0);
};

