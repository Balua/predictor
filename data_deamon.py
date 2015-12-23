import subprocess
import datetime



website = "http://nomads.ncep.noaa.gov/cgi-bin/filter_gfs_hd.pl?file=gfs.t12z.mastergrb2f00&all_lev=on&var_HGT=on&var_PRES=on&var_RH=on&var_TMP=on&var_UGRD=on&var_VGRD=on&leftlon=0&rightlon=360&toplat=90&bottomlat=-90&dir=%2Fgfs.2014102612%2Fmaster"
dataset = "noaa_wind_t18zf192"


ret = subprocess.call(["wget","-O",dataset, website ])
