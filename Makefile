all:
	g++ -Wall -Wextra -std=c++11 main.cpp weather.cpp predictor.cpp -o predictor -I /home/Documents/libraries/includes -L /home/Documents/libraries/lib -lgrib_api -lm


release:
	g++ -Wall -Wextra -std=c++11 -Ofast main.cpp weather.cpp predictor.cpp -o predictor -I /home/Documents/libraries/includes -L /home/Documents/libraries/lib -lgrib_api -lm


clean:
	$(RM) predictor



