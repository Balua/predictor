#!/usr/bin/python 

# the objective of this script is to fetch wind data from NOAA pydap servers and parse it in a convenient output format for analysis by a faster C routine.


import logging
import sys

# creating logging information
logger = logging.getLogger('predictor.py')
logger.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

logger.info('Running')


try:
	import numpy 
	import pydap 

except Exception, err:
	logger.critical(err)
	sys.exit(err)








