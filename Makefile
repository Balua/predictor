all:
	g++ -Wall -Wextra -std=c++11 main.cpp weather.cpp predictor.cpp -o predictor -I /home/Documents/libraries/grib_api/include -L /home/Documents/libraries/grib_api/lib -lgrib_api -lm


release:
	g++ -Wall -Wextra -std=c++11 -Ofast main.cpp weather.cpp predictor.cpp -o predictor -I /home/Documents/libraries/grib_api/include -L /home/Documents/libraries/grib_api/lib -lgrib_api -lm


clean:
	$(RM) predictor



