
#include "weather.h"      
#include <iostream>

//destructor
Weather_Dataset::~Weather_Dataset()
{
	//expressely unload file
	unload();
}


void Weather_Dataset::get_wind(struct pred_block *cur_block)
{
    //temporary bypass values
    cur_block->v_wind = 1.0;
    cur_block->u_wind = 1.0;
}


void Weather_Dataset::load(std::string p_filename)
{
	//set filename
	m_filename = p_filename;

    int err = 0;

    m_file_handle = fopen(m_filename.c_str(),"r");
    if(m_file_handle)
    {
        std::cout << "INFO: opened file: "<< m_filename << std::endl;
    }
    else
    {
        std::cout << "ERROR: unable to open file "<< m_filename << std::endl;
        return;
    }

        
    //create new handle from a message in a file
    m_grib_handle = grib_handle_new_from_file(0,m_file_handle,&err);
    if (m_grib_handle == NULL)
    {
        std::cout << "ERROR: unable to create handle " << std::endl;
        return;
    }
    else
    {
        std::cout << "INFO: created grib handle"<< std::endl;
    }

}

void Weather_Dataset::unload()
{
    //delete grib handle
    if(grib_handle_delete(m_grib_handle) == 0)
    {
        std::cout << "INFO: deleted grib handle"<< std::endl;
    }
    else
    {
        std::cout << "ERROR: unable to delete handle " << std::endl;
        return;
    }


    //close file
    if(fclose(m_file_handle) == 0 )
    {
        std::cout << "INFO: closed file "<< m_filename << std::endl;
    }
    else
    {
        std::cout << "ERROR: closing file "<< m_filename << std::endl;
        return;
    }
}

void Weather_Dataset::task()
{
	long int N_lat;
	long int N_lon;
	double   lat_min, lat_max;
	double   lon_min, lon_max;
	double   jDirectionIncrementInDegrees;
	double   iDirectionIncrementInDegrees;

	grib_get_long(m_grib_handle, "Ni", &N_lat);
	grib_get_long(m_grib_handle, "Nj", &N_lon);
	grib_get_double(m_grib_handle, "latitudeOfFirstGridPointInDegrees", &lat_min);
	grib_get_double(m_grib_handle, "longitudeOfFirstGridPointInDegrees", &lon_min);
	grib_get_double(m_grib_handle, "latitudeOfLastGridPointInDegrees", &lat_max);
	grib_get_double(m_grib_handle, "longitudeOfLastGridPointInDegrees", &lon_max);

	grib_get_double(m_grib_handle,"iDirectionIncrementInDegrees",&iDirectionIncrementInDegrees);
	grib_get_double(m_grib_handle,"jDirectionIncrementInDegrees",&jDirectionIncrementInDegrees);

	std::cout << "numberOfPointsAlongAParallel=" << N_lat << std::endl;
	std::cout << "numberOfPointsAlongAMeridian=" << N_lon << std::endl;
	std::cout << "latitudeOfFirstGridPointInDegrees=" << lat_min << std::endl;
	std::cout << "longitudeOfFirstGridPointInDegrees=" << lon_min << std::endl;

	std::cout << "iDirectionIncrementInDegrees="  << iDirectionIncrementInDegrees << std::endl;
	std::cout << "jDirectionIncrementInDegrees="  << jDirectionIncrementInDegrees << std::endl;

}
