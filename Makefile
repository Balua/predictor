
#define dependencies paths
PUGIXML_SRC  = ${CURDIR}/dependencies/pugixml/src




all:
	g++ -Wall -Wextra -std=c++11 -fprofile-arcs -ftest-coverage  main.cpp $(PUGIXML_SRC)/pugixml.cpp predictor.cpp weather.cpp -o predictor -I $(PUGIXML_SRC) -L -l -lgrib_api -lgcov
	

release:
	g++ -Wall -Wextra -std=c++11 -0fast main.cpp $(PUGIXML_SRC)/pugixml.cpp predictor.cpp weather.cpp -o predictor -I $(PUGIXML_SRC) -L -l -lgrib_api


clean:
	$(RM) predictor



