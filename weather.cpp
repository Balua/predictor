
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
	long int Nlat;
	long int Nlon;
	double   lat_min, lat_max, dlat;
	double   lon_min, lon_max, dlon;



	grib_get_long(m_grib_handle, "Ni", &Nlat);
	grib_get_long(m_grib_handle, "Nj", &Nlon);
	grib_get_double(m_grib_handle, "latitudeOfFirstGridPointInDegrees", &lat_min);
	grib_get_double(m_grib_handle, "longitudeOfFirstGridPointInDegrees", &lon_min);
	grib_get_double(m_grib_handle, "latitudeOfLastGridPointInDegrees", &lat_max);
	grib_get_double(m_grib_handle, "longitudeOfLastGridPointInDegrees", &lon_max);

	grib_get_double(m_grib_handle,"iDirectionIncrementInDegrees",&dlat);
	grib_get_double(m_grib_handle,"jDirectionIncrementInDegrees",&dlon);

	std::cout << "numberOfPointsAlongAParallel=" << Nlat << std::endl;
	std::cout << "numberOfPointsAlongAMeridian=" << Nlon << std::endl;
	std::cout << "latitudeOfFirstGridPointInDegrees=" << lat_min << std::endl;
	std::cout << "longitudeOfFirstGridPointInDegrees=" << lon_min << std::endl;

	std::cout << "iDirectionIncrementInDegrees="  << dlat << std::endl;
	std::cout << "jDirectionIncrementInDegrees="  << dlon << std::endl;



	std::cout << "----------" << std::endl;

	size_t len;
	char parameterName[30];
	grib_get_string(m_grib_handle,"name",parameterName,&len);
	std::cout << "parameterName=" << parameterName << " len=" << len << std::endl;

	grib_get_string(m_grib_handle,"units",parameterName,&len);
	std::cout << "units=" << parameterName << " len=" << len << std::endl;

	long level;
	grib_get_long(m_grib_handle, "level", &level);
	std::cout << "level=" << level << std::endl;

	std::cout << "----------" << std::endl;


//	hgtprs  geopotential height [gpm]
//	 Grid {
//	    ARRAY:
//	      Float32 hgtprs[time = 81][lev = 26][lat = 721][lon = 1440];
//	    MAPS:
//	      Float64 time[time = 81];
//	      Float64 lev[lev = 26];
//	      Float64 lat[lat = 721];
//	      Float64 lon[lon = 1440];
//	  } hgtprs;
//	U-component of wind [m/s]
//	Grid {
//	    ARRAY:
//	      Float32 ugrdprs[time = 81][lev = 26][lat = 721][lon = 1440];
//	    MAPS:
//	      Float64 time[time = 81];
//	      Float64 lev[lev = 26];
//	      Float64 lat[lat = 721];
//	      Float64 lon[lon = 1440];
//	  } ugrdprs;
//	V-component of wind [m/s]
//	Grid {
//		ARRAY:
//		  Float32 vgrdprs[time = 81][lev = 26][lat = 721][lon = 1440];
//		MAPS:
//		  Float64 time[time = 81];
//		  Float64 lev[lev = 26];
//		  Float64 lat[lat = 721];
//		  Float64 lon[lon = 1440];
//	  } vgrdprs;





}
