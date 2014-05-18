all:
	g++-4.8 -Wall -Wextra -std=c++11 main.cpp weather.cpp predictor.cpp -o predictor


release:
	g++-4.8 -Wall -Wextra -std=c++11 -Ofast main.cpp weather.cpp predictor.cpp -o predictor


clean:
	$(RM) predictor



