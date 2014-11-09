#include "predictor.h"


void Weather_Dataset::get_wind(struct pred_block *cur_block)
{
    //temporary bypass values
    cur_block->v_wind = 1.0;
    cur_block->u_wind = 1.0;
}



void Weather_Dataset::load()
{
    int err = 0;
    FILE* in = NULL;
    grib_handle *h = NULL;

    in = fopen(Weather_Dataset::file_name.c_str(),"r");
    if(in) 
    {
        std::cout << "INFO: opened file: "<< Weather_Dataset::file_name << std::endl;
    }
    else
    {
        std::cout << "ERROR: unable to open file "<< Weather_Dataset::file_name << std::endl;
        return;
    }

        
    //create new handle from a message in a file
    h = grib_handle_new_from_file(0,in,&err);
    if (h == NULL) 
    {
        std::cout << "ERROR: unable to create handle " << std::endl;
        return;
    }
    else
    {
        std::cout << "INFO: created grib handle"<< std::endl;
    }


    //delete grib handle
    if(grib_handle_delete(h) == 0)
    {
        std::cout << "INFO: deleted grib handle"<< std::endl;
    }
    else
    {
        std::cout << "ERROR: unable to delete handle " << std::endl;
        return;
    }


    //close file
    if(fclose(in) == 0 )
    {
        std::cout << "INFO: closed file "<< Weather_Dataset::file_name << std::endl;
    }
    else
    {
        std::cout << "ERROR: closing file "<< Weather_Dataset::file_name << std::endl;
        return;
    }


}





