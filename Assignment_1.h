#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

//Grid info struct
struct GridInfo
{
	int xPos;
	int yPos;
	int cityID;
	int cc;
	int ap;
	string cityName;

	GridInfo()
	{
		xPos = 0;
		yPos = 0;
		cityID = 0;
		cc = -1;
		ap = -1;
		cityName = "";
	}
};

//City info struct
struct CityInfo
{
	int id;
	string name;
	vector<GridInfo*> cityGrids;
	vector<GridInfo*> adjacentGrids;

	CityInfo()
	{
		id = 0;
		name = "";
	}
};

//Initial config and file input
//=======================================//
string fileToLoad;
string CLFile;
string CCFile;
string APFile;
vector<string> configLines; //Vector that stores text extracted from config file
vector<CityInfo*> cityInfoList; //Vector that stores the information of each city
//=======================================//
GridInfo ** gridTileInfoArray = nullptr; //2D array that stores the data of each tile on the map
bool configLoaded; //Boolean to check if all config files have been loaded successfully
//=======================================//
//Menu display
bool displayingOutput;
int selectedOption;
//=======================================//
//Map display
int xMin, xMax; //Map min x and max x pos
int yMin, yMax; //Map min y and max y pos
int xOffset; //Offset for when range does not start from zero
int yOffset; //Offset for when range does not start from zero
int gridRow = 0; //Number of rows for entire grid 
int gridCol = 0; //Number of cols for entire grid 
int mapRow = 0; //Number of rows for the actual map
int mapCol = 0; //Number of cols for the actual map
string** gridArray = nullptr; //2D array for displaying the map
//=======================================//
void clearDataContainers(); //Function to empty data containers before loading a config file

void readConfigFiles(); //Function for reading the config file

void readCLFile(string); //Function to read city location file

void readCCIndexFile(string); //Function to read cloud coverage file

void readAPIndexFile(string); //Function to read atmospheric pressure file

void generateCityInfo(); //Function to populate the city info list

void initConfig(); //Function for initializing the map array, assigning variables etc.

void showGrid(); //Iterates through map array and prints out the stored string at each position to the console

void clearGrid(); //Clears the displayed grid array

void displayMap(); //Outputs the grid bounding box and range values to the grid array

void displayCityMap(); //Display city location map

void displayCCMapIndex(); //Display cloud coverage index map

void displayCCMapLMH(); //Display cloud coverage index map in LMH

void displayAPMapIndex(); //Display atmospheric pressure index map

void displayAPMapLMH(); //Display atmospheric pressure index map in LMH

void displayWeatherReport(); //Outputs weather report for all cities

float calculateAverageCC(CityInfo*); //Calculate average CC

float calculateAverageAP(CityInfo*); //Calculate average AP

float calculateRainProb(string, string); //Calculate rain prob

void displayRainProb(string, string); //Display ASCII

void deallocMemory(); //Function to deallocate memory

void Quit(); //Closes application


