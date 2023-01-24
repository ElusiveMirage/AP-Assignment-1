#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
		cc = 0;
		ap = 0;
		cityName = "";
	}
};

//Initial config and file input
string fileToLoad;
string CLFile;
string CCFile;
string APFile;
vector<string> configLines;
//=======================================//
GridInfo ** mapTileInfoArray = nullptr;

bool configLoaded;


//Menu display
bool displayingMap;
int selectedOption;

//Map display
int xMin, xMax; //Map min x and max x pos
int yMin, yMax; //Map min y and max y pos
int xMinOffset;
int yMinOffset;
int gridRow = 0; //Number of rows for entire grid 
int gridCol = 0; //Number of cols for entire grid 
int mapRow = 0; //Number of rows for the actual map
int mapCol = 0; //Number of cols for the actual map
string** gridArray = nullptr; //2D array for displaying the map

void readConfigFiles(string); //Function for reading the config file

void readCLFile(string);

void readCCIndexFile(string);

void initConfig(); //Function for initializing the map array, assigning variables etc.

void showGrid(); //Iterates through map array and prints out the stored string at each position to the console

void displayMap();

void displayCityMap();

void displayCCMapIndex();

void displayCCMapLMH();

void displayAPMapIndex();

void displayAPMapLMH();

void displayWeatherReport();

void deallocMemory();

void Quit();
