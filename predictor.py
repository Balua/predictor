#-coding utf8
#!/usr/bin/python 

# the objective of this script is to fetch wind data from NOAA pydap servers and parse it in a convenient output format for analysis by a faster C routine.

# all shit

import logging
import sys
import datetime
import time
import pydap.client, pydap.exceptions
import math

# creating logging information
logger = logging.getLogger('predictor.py')
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)
logger.info('Running')



def GetNearDataset(utc_time=datetime.datetime.utcnow()):
    logger.info("Current UTC Time @ %s"%utc_time)
    #getting the nomads dataset release time
    release_time=datetime.datetime(utc_time.year,utc_time.month,utc_time.day,18,00,0)
    while utc_time< release_time:
        release_time=release_time -datetime.timedelta(hours=6)
    
    dataset=0
    #connecting to noaa pydap server and getting the first available dataset closest to current UTC time
    dataset_status=False
    while dataset_status==False:
        try:
            #formatting url
            url= 'http://nomads.ncep.noaa.gov:9090/dods/gfs_hd/gfs_hd%i%02i%02i/gfs_hd_%02iz'%(release_time.year, release_time.month, release_time.day, release_time.hour)
            dataset=pydap.client.open_url(url)
            dataset_status=True
            logger.info("Got Dataset @ %s"%url)
        except pydap.exceptions.ServerError:
            logger.warning("Unavailable Dataset @ %s"%url)
            release_time=release_time -datetime.timedelta(hours=6)
    return dataset 


dataset=GetNearDataset(datetime.datetime.now())
#
#
##no idea what is this for
#time_origin=origin=datetime.datetime(1,1,1,0,0,0)  #1AC Jan 1
#
#for i in dataset.time:
#	print time_origin+datetime.timedelta(i-2) # have to subtract 2 days since 1AC Jan 0 
#	break
#
#
#print dataset.hgtprs.dimensions
#print dataset.hgtprs.shape
#print dataset.ugrdprs.dimensions
#print dataset.ugrdprs.shape
#print dataset.vgrdprs.dimensions
#print dataset.vgrdprs.shape











