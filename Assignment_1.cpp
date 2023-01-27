// Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Assignment_1.h"

int main()
{
    while (true) {

        if (!displayingOutput)
        {
            cout << endl;
            cout << "Student ID   : 7895379" << endl;
            cout << "Student Name : Tan Jing Jie Javier" << endl;
            cout << "[ Welcome to Virtual Solutions Weather Information Procesing System ]" << endl;
            cout << "=====================================================================" << endl;
            cout << "1) Read in and process a configuration file" << endl;
            cout << "2) Display city map" << endl;
            cout << "3) Display cloud coverage map (cloudiness index)" << endl;
            cout << "4) Display cloud coverage map (LMH)" << endl;
            cout << "5) Display atmospheric pressure map (pressure index)" << endl;
            cout << "6) Display atmospheric pressure map (LMH)" << endl;
            cout << "7) Display weather forecast summary report" << endl;
            cout << "8) Exit application" << endl;
            cout << endl;
            cout << "Please select an option to proceed : ";

            cin >> selectedOption;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid input, please try again!" << endl;
            }
            else
            {
                if (selectedOption >= 1 && selectedOption <= 8)
                {
                    cout << endl;

                    switch (selectedOption)
                    {
                    case 1:
                        cout << "[ Read in and process a configuration file ]" << endl;
                        readConfigFiles();
                        break;
                    case 2:
                        cout << "[ Display city map ]" << endl;
                        displayCityMap();
                        break;
                    case 3:
                        cout << "[ Display cloud coverage map (cloudiness index) ]" << endl;
                        displayCCMapIndex();
                        break;
                    case 4:
                        cout << "[ Display cloud coverage map (LMH) ]" << endl;
                        displayCCMapLMH();
                        break;
                    case 5:
                        cout << "[ Display atmospheric pressure map (pressure index) ]" << endl;
                        displayAPMapIndex();
                        break;
                    case 6:
                        cout << "[ Display atmospheric pressure map (LMH) ]" << endl;
                        displayAPMapLMH();
                        break;
                    case 7:
                        cout << "[ Display weather forecast summary report ]" << endl;
                        displayWeatherReport();
                        break;
                    case 8:
                        cout << "[ Exit application ]" << endl;
                        Quit();
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        else
        {
            string str = "";
            cin.clear();
            cin.ignore();

            cout << "Press <enter> to go back to main menu...";

            cin.ignore();
           
            if (str.length() == 0)
            {
                displayingOutput = false;
            }
        }
    }
}

void initConfig()
{
    xMax = stoi(configLines[0].substr(configLines[0].find('-') + 1));
    xMin = stoi(configLines[0].substr(configLines[0].find('=') + 1, configLines[0].find('-')));
    yMax = stoi(configLines[1].substr(configLines[1].find('-') + 1));
    yMin = stoi(configLines[1].substr(configLines[1].find('=') + 1, configLines[0].find('-')));

    cout << "Grid ranges set..." << endl;

    CLFile = configLines[2];
    CCFile = configLines[3];
    APFile = configLines[4];

    gridCol = ((xMax - xMin) + 1) + 3;
    gridRow = ((yMax - yMin) + 1) + 3;
    mapCol = (xMax - xMin) + 1;
    mapRow = (yMax - yMin) + 1;

    if (xMin > 0)
    {
        xOffset = xMin;
    }
    else
    {
        xOffset = 0;
    }

    if (yMin > 0)
    {
        yOffset = yMin;
    }
    else
    {
        yOffset = 0;
    }

    if (gridRow > 0)
    {
        gridTileInfoArray = new GridInfo * [mapRow];
        int count = 0;
        for (int i = 0; i < mapRow; i++)
        { 
            gridTileInfoArray[i] = new GridInfo[mapCol];
        }

        for (int i = 0; i < mapRow; i++)
        {
            for (int j = 0; j < mapCol; j++)
            {
                GridInfo* temp = new GridInfo();

                gridTileInfoArray[i][j] = *temp;
            }
        }

        gridArray = new string * [gridRow];

        for (int i = 0; i < gridRow; i++)
        {
            gridArray[i] = new string[gridCol];
        }
    }

    //read in data files
    readCLFile(CLFile);
    readCCIndexFile(CCFile);
    readAPIndexFile(APFile);

    cout << "All configuration files loaded successfully. Returning to main menu..." << endl;

    generateCityInfo();

    displayMap();

    configLoaded = true;
}

void clearDataContainers()
{
    configLines.clear();
    cityInfoList.clear();
    gridTileInfoArray = new GridInfo * [mapRow];
    gridArray = new string * [gridRow];
}

bool isEnterPressed()
{
    string str;
    
    getline(cin, str);
    if (str == "") {
        return true;
    }
    return false;
}

void readConfigFiles()
{
    cout << "Please enter config file name" << endl;
    
    clearDataContainers();

    string filename;

    cin >> filename;

    fstream inputFile(filename.c_str(), fstream::in);

    if (inputFile.good())
    {
        cout << endl;
        cout << "Reading contents of file : " << filename << endl;

        string aLine;

        while (getline(inputFile, aLine))
        {
            //cout << aLine << endl;

            if (aLine == "") continue;

            if (aLine[0] != '/')
            {
                configLines.push_back(aLine);
            }
        }
        cout << endl;

        initConfig();
    }
    else
    {
        cout << "Config file does not exist!" << endl;
    }
}

void readCLFile(string filename)
{
    fstream inputFile(filename.c_str(), fstream::in);

    if (inputFile.good())
    {
        cout << endl;
        cout << "Reading contents of file : " << filename << endl;
        cout << endl;

        string aLine;

        vector<string> cityList;
        vector<int> idList;

        while (getline(inputFile, aLine))
        {
            //cout << aLine << endl;

            if (aLine == "") continue;

            int x = stoi(aLine.substr(aLine.find('[') + 1, aLine.find(',')));
            int y = stoi(aLine.substr(aLine.find(',') + 2, aLine.find(']')));
            int id = stoi(aLine.substr(aLine.find('-') + 1, aLine.find_last_of('-')));
            string name = aLine.substr(aLine.find_last_of('-') + 1);
 
            if (y - yOffset > mapRow || x - xOffset > mapCol)
            {
                cout << "WARNING : Value" << "[ " << x << ", " << y << " ]" << "out of range of map bounds" << endl;
                continue;
            }

            if (find(cityList.begin(), cityList.end(), name) == cityList.end())
            {
                cityList.push_back(name);
                idList.push_back(id);
            }

            gridTileInfoArray[y - yOffset][x - xOffset].xPos = x;
            gridTileInfoArray[y - yOffset][x - xOffset].yPos = y;
            gridTileInfoArray[y - yOffset][x - xOffset].cityID = id;
            gridTileInfoArray[y - yOffset][x - xOffset].cityName = name;
        }

        for (int i = 0; i < cityList.size(); i++)
        {
            CityInfo* info = new CityInfo();
            info->name = cityList[i];
            info->id = idList[i];
            cityInfoList.push_back(info);
        }

        cout << "City locations loaded..." << endl;
    }
    else
    {
        cout << "City location file does not exist!" << endl;
    }
}

void readCCIndexFile(string filename)
{
    fstream inputFile(filename.c_str(), fstream::in);

    if (inputFile.good())
    {
        cout << endl;
        cout << "Reading contents of file : " << filename << endl;
        cout << endl;

        string aLine;

        while (getline(inputFile, aLine))
        {
            //cout << aLine << endl;

            if (aLine == "") continue;

            int x = stoi(aLine.substr(aLine.find('[') + 1, aLine.find(',')));
            int y = stoi(aLine.substr(aLine.find(',') + 2, aLine.find(']')));
            int cc = stoi(aLine.substr(aLine.find('-') + 1));

            if (y - yOffset > mapRow || x - xOffset > mapCol)
            {
                cout << "WARNING : Value" << "[ " << x << ", " << y << " ]" << "out of range of map bounds" << endl;
                continue;
            }

            gridTileInfoArray[y - yOffset][x - xOffset].xPos = x;
            gridTileInfoArray[y - yOffset][x - xOffset].yPos = y;
            gridTileInfoArray[y - yOffset][x - xOffset].cc = cc;
        }

        cout << "Cloud coverage data loaded..." << endl;
    }
    else
    {
        cout << "Cloud cover file does not exist!" << endl;
    }
}

void readAPIndexFile(string filename)
{
    fstream inputFile(filename.c_str(), fstream::in);

    if (inputFile.good())
    {
        cout << endl;
        cout << "Reading contents of file : " << filename << endl;
        cout << endl;

        string aLine;

        while (getline(inputFile, aLine))
        {
            //cout << aLine << endl;

            if (aLine == "") continue;

            int x = stoi(aLine.substr(aLine.find('[') + 1, aLine.find(',')));
            int y = stoi(aLine.substr(aLine.find(',') + 2, aLine.find(']')));
            int ap = stoi(aLine.substr(aLine.find('-') + 1));

            if (y - yOffset > mapRow || x - xOffset > mapCol)
            {
                cout << "WARNING : Value" << "[ " << x << ", " << y << " ]" << "out of range of map bounds" << endl;
                continue;
            }

            gridTileInfoArray[y - yOffset][x - xOffset].xPos = x;
            gridTileInfoArray[y - yOffset][x - xOffset].yPos = y;
            gridTileInfoArray[y - yOffset][x - xOffset].ap = ap;
        }

        cout << "Atmospheric pressure data loaded..." << endl;
    }
    else
    {
        cout << "Atmospheric pressure file does not exist!" << endl;
    }
}

void generateCityInfo()
{
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].cityName != "")
            {
                for (CityInfo* info : cityInfoList)
                {
                    if (info->name == gridTileInfoArray[i][j].cityName)
                    {
                        info->cityGrids.push_back(&gridTileInfoArray[i][j]); //push pointer to city tile to vector
                    }                  
                }

                //Check all 8 directions and push pointer to grid info of surrounding tiles to vector
                for (int y = -1; y < 2; y++)
                {
                    for (int x = -1; x < 2; x++)
                    {
                        if (x == 0 && y == 0)
                            continue;

                        if (i + y > yMax - yOffset || j + x > xMax - xOffset || i + y < yMin - yOffset || j + x < xMin - xOffset) //if adjacent tile outside map bounds, ignore
                        {
                            continue; 
                        }

                        if (gridTileInfoArray[i + y][j + x].cityName == gridTileInfoArray[i][j].cityName) //if adjacent tile is part of current tile's city, ignore
                        {
                            continue;
                        }
                        else
                        {
                            for (CityInfo* info : cityInfoList)
                            {
                                if (info->name == gridTileInfoArray[i][j].cityName)
                                {
                                    if (find(info->adjacentGrids.begin(), info->adjacentGrids.end(), &gridTileInfoArray[i + y][j + x]) != info->adjacentGrids.end())
                                    {
                                        continue; //skip if pointer to tile already exists in the vector
                                    }
                                    else
                                        info->adjacentGrids.push_back(&gridTileInfoArray[i + y][j + x]); //push pointer to adjacent tile into vector
                                
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void showGrid()
{
    cout << endl;

    for (int i = gridRow - 1; i > -1; i--)
    {
        for (int j = 0; j < gridCol; j++)
        {
            if (gridArray[i][j] == "")
            {
                cout << "   ";
            }
            else
                cout << gridArray[i][j];

        }
        cout << endl;
    }

    displayingOutput = true;
}

void clearGrid() 
{
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            gridArray[i + 2][j + 2] = "";
        }
    }
}

void displayMap()
{
    for (int i = gridRow - 1; i > -1; i--)
    {
        for (int j = 0; j < gridCol; j++)
        {
            //Range numbers
            if (i == 0)
            {
                if (j > 1 && j < gridCol - 1)
                {
                    int temp = j + yOffset - 2;

                    if (temp >= 10)
                        gridArray[i][j] = to_string((temp)) + " ";
                    else
                        gridArray[i][j] = to_string((temp)) + "  ";
                }
            }

            if (j == 0)
            {
                if (i > 1 && i < gridRow - 1)
                {
                    int temp2 = i + xOffset - 2;

                    if(temp2 >= 10)
                        gridArray[i][j] = to_string((temp2)) + " ";
                    else
                        gridArray[i][j] = to_string((temp2)) + "  ";
                }
            }

            //Grid box
            if (i == gridRow - 1 || i == 1)
            {
                if(j != 0)
                    gridArray[i][j] = "#  ";
            }                     
            
            if (j == 1 || j == gridCol - 1)
            {
                if(i > 1 && i < gridRow - 1)
                    gridArray[i][j] = "#  ";
            }
        }
    }
}

void displayCityMap()
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    clearGrid();

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].cityName != "")
            {
                if (i + 2 <= gridRow - 2 && j + 2 <= gridCol - 2)
                {
                    gridArray[i + 2][j + 2] = to_string(gridTileInfoArray[i][j].cityID) + "  ";
                }
            }
        }
    }

    showGrid();
}

void displayCCMapIndex()
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    clearGrid();

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            string temp;

            if (gridTileInfoArray[i][j].cc != -1)
            {
                if (gridTileInfoArray[i][j].cc >= 0 && gridTileInfoArray[i][j].cc < 10)
                {
                    temp = "0";
                }
                else if (gridTileInfoArray[i][j].cc >= 10 && gridTileInfoArray[i][j].cc < 100)
                {
                    temp = to_string(gridTileInfoArray[i][j].cc)[0];
                }

                if (i + 2 <= gridRow - 2 && j + 2 <= gridCol - 2)
                {
                    gridArray[i + 2][j + 2] = temp + "  ";
                }
            }           
        }
    }

    showGrid();
}

void displayCCMapLMH()
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    clearGrid();

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            string temp;

            if (gridTileInfoArray[i][j].cc != -1)
            {
                if (gridTileInfoArray[i][j].cc >= 0 && gridTileInfoArray[i][j].cc < 35)
                {
                    temp = "L";
                }
                else if (gridTileInfoArray[i][j].cc >= 35 && gridTileInfoArray[i][j].cc < 65)
                {
                    temp = "M";
                }
                else if (gridTileInfoArray[i][j].cc >= 65 && gridTileInfoArray[i][j].cc < 100)
                {
                    temp = "H";
                }

                if (i + 2 <= gridRow - 2 && j + 2 <= gridCol - 2)
                {
                    gridArray[i + 2][j + 2] = temp + "  ";
                }
            }
        }
    }

    showGrid();
}

void displayAPMapIndex() //Displays atmospheric pressure value on map
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    clearGrid();

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            string temp;

            if (gridTileInfoArray[i][j].ap != -1)
            {
                if (gridTileInfoArray[i][j].ap <= 0 || gridTileInfoArray[i][j].ap < 10)
                {
                    temp = "0";
                }

                if (gridTileInfoArray[i][j].ap >= 10 && gridTileInfoArray[i][j].ap < 100)
                {
                    temp = to_string(gridTileInfoArray[i][j].ap)[0];
                }

                if (i + 2 <= gridRow - 2 && j + 2 <= gridCol - 2)
                {
                    gridArray[i + 2][j + 2] = temp + "  ";
                }
            }
        }
    }

    showGrid();
}

void displayAPMapLMH() //Displays atmospheric pressure value on map in LMH
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    clearGrid();

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            string temp;

            if (gridTileInfoArray[i][j].ap != -1)
            {
                if (gridTileInfoArray[i][j].ap >= 0 && gridTileInfoArray[i][j].ap < 35)
                {
                    temp = "L";
                }
                else if (gridTileInfoArray[i][j].ap >= 35 && gridTileInfoArray[i][j].ap < 65)
                {
                    temp = "M";
                }
                else if (gridTileInfoArray[i][j].ap >= 65 && gridTileInfoArray[i][j].ap < 100)
                {
                    temp = "H";
                }

                if (i + 2 <= gridRow - 2 && j + 2 <= gridCol - 2)
                {
                    gridArray[i + 2][j + 2] = temp + "  ";
                }
            }
        }
    }

    showGrid();
}

void displayWeatherReport() //Displays weather forecast summary report for each city on map
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    displayingOutput = true;

    cout << endl;
    cout << "[ Weather Forecast Summary Report ]" << endl;
    cout << "===================================" << endl;

    for (CityInfo* city : cityInfoList)
    {
        float averageCC = calculateAverageCC(city);
        float averageAP = calculateAverageAP(city);

        string CCSymbol;
        string APSymbol;

        if (averageCC >= 0 && averageCC < 35)
        {
            CCSymbol = "L";
        }
        if (averageCC >= 35 && averageCC < 65)
        {
            CCSymbol = "M";
        }
        if (averageCC >= 65 && averageCC < 100)
        {
            CCSymbol = "H";
        }

        if (averageAP >= 0 && averageAP < 35)
        {
            APSymbol = "L";
        }
        if (averageAP >= 35 && averageAP < 65)
        {
            APSymbol = "M";
        }
        if (averageAP >= 65 && averageAP < 100)
        {
            APSymbol = "H";
        }

        cout << "City Name : " << city->name << endl;
        cout << "City ID   : " << city->id << endl;
        cout << "Ave. Cloud Cover (ACC)  : " << fixed << setprecision(2) << averageCC << "  ( " << CCSymbol << " )" << endl;
        cout << "Ave. Pressure (AP)      : " << fixed << setprecision(2) << averageAP << "  ( " << APSymbol << " )" << endl;
        cout << "Probability of Rain (%) : " << calculateRainProb(CCSymbol, APSymbol) << endl;

        displayRainProb(CCSymbol, APSymbol);

        cout << endl;
    }
}

float calculateAverageCC(CityInfo *city)
{
    float CC = 0;
    int cityCC = 0;
    int surroundingCC = 0;
    int tilesToIgnore = 0;

    for (GridInfo* grid : city->cityGrids)
    {
        cityCC += grid->cc;
    }

    for (GridInfo* grid : city->adjacentGrids)
    {
        if (grid->cc == -1)
        {
            tilesToIgnore++;
            continue;
        }

        surroundingCC += grid->cc;
    }

    CC = (float)(cityCC + surroundingCC) / (float)(city->cityGrids.size() + city->adjacentGrids.size() - tilesToIgnore);

    return CC;
}

float calculateAverageAP(CityInfo *city)
{
    float AP = 0;
    int cityAP = 0;
    int surroundingAP = 0;
    int tilesToIgnore = 0;

    for (GridInfo* grid : city->cityGrids)
    {
        cityAP += grid->ap;
    }

    for (GridInfo* grid : city->adjacentGrids)
    {
        if (grid->ap == -1)
        {
            tilesToIgnore++;
            continue;
        }

        surroundingAP += grid->ap;
    }

    AP = (float)(cityAP + surroundingAP) / (float)(city->cityGrids.size() + city->adjacentGrids.size() - tilesToIgnore);

    return AP;
}

float calculateRainProb(string cc, string ap)
{
    float rainProb = 0.0f;

    if (ap == "L")
    {
        if (cc == "H")
        {
            rainProb = 90.00f;
        }
        else if (cc == "M")
        {
            rainProb = 80.00f;
        }
        else if (cc == "L")
        {
            rainProb = 70.00f;
        }
    }
    else if (ap == "M")
    {
        if (cc == "H")
        {
            rainProb = 60.00f;
        }
        else if (cc == "M")
        {
            rainProb = 50.00f;
        }
        else if (cc == "L")
        {
            rainProb = 40.00f;
        }
    }
    else if (ap == "H")
    {
        if (cc == "H")
        {
            rainProb = 30.00f;
        }
        else if (cc == "M")
        {
            rainProb = 20.00f;
        }
        else if (cc == "L")
        {
            rainProb = 10.00f;
        }
    }

    return rainProb;
}

void displayRainProb(string cc, string ap)
{
    if (ap == "L")
    {
        cout << "~  ~  ~  ~" << endl;
        cout << "~  ~  ~  ~  ~" << endl;

        if(cc == "H")
        {
            cout << "\\ \\ \\ \\ \\ " << endl;

        }
        else if (cc == "M")
        {
            cout << "  \\ \\ \\ \\ " << endl;
        }
        else if (cc == "L")
        {
            cout << "     \\ \\ \\ " << endl;
        }
    }
    else if (ap == "M")
    {
        cout << "~  ~  ~  ~" << endl;
        cout << "~  ~  ~  ~  ~" << endl;

        if (cc == "H")
        {
            cout << "        \\ \\ " << endl;
        }
        else if (cc == "M")
        {
            cout << "           \\ " << endl;
        }
    }
    else if (ap == "L")
    {
        if (cc == "H")
        {
            cout << "~  ~  ~" << endl;
            cout << "~  ~  ~  ~" << endl;
        }
        else if (cc == "M")
        {
            cout << "~  ~" << endl;
            cout << "~  ~  ~" << endl;
        }
        else if (cc == "L")
        {
            cout << "~" << endl;
            cout << "~  ~" << endl;
        }
    }
}

void deallocMemory()
{
    if (mapCol <= 0)
        return;

    for (int i = 0; i < mapRow; i++)
    {
        cout << "Pointer array at memory address " << &gridTileInfoArray[i] << " deleted" << endl;
        delete[] gridTileInfoArray[i]; 
    }
    
    cout << "Pointer array at memory address " << &gridTileInfoArray << " deleted" << endl;
    delete[] gridTileInfoArray;

    if (gridCol <= 0)
        return;

    for (int i = 0; i < gridRow; i++)
    {
        cout << "Pointer array at memory address " << &gridArray[i] << " deleted" << endl;
        delete[] gridArray[i];
    }

    cout << "Pointer array at memory address " << &gridArray << " deleted" << endl;
    delete[] gridArray;
}

void Quit()
{
    deallocMemory();

    exit(0);
}



