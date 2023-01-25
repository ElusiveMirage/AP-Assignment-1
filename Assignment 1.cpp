// Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Assignment 1.h"

int main()
{
    //while (!configLoaded)
    //{
    //    displayingMap = false;

    //    cout << "Please enter config file name" << endl;

    //    cin >> fileToLoad;

    //    readConfigFiles(fileToLoad);
    //    
    //    initConfig();

    //    displayMap();
    //}

    while (true) {

        if (!displayingMap)
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
        else if (displayingMap)
        {
            char ch;

            cout << "Press <enter> to return to the main menu..." << endl;

            ch = _getch();

            if (cin.get() == '\n')
                displayingMap = false;
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

void initConfig()
{
    xMax = stoi(configLines[0].substr(configLines[0].find('-') + 1));
    xMin = stoi(configLines[0].substr(configLines[0].find('=') + 1, configLines[0].find('-')));
    yMax = stoi(configLines[1].substr(configLines[1].find('-') + 1));
    yMin = stoi(configLines[1].substr(configLines[1].find('=') + 1, configLines[0].find('-')));

    CLFile = configLines[2];
    CCFile = configLines[3];
    APFile = configLines[4];

    gridCol = ((xMax - xMin) + 1) + 3;
    gridRow = ((yMax - yMin) + 1) + 3;
    mapCol = (xMax - xMin) + 1;
    mapRow = (yMax - yMin) + 1;

    if (xMin > 0)
    {
        xMinOffset = xMin;
    }
    else
    {
        xMinOffset = 0;
    }

    if (yMin > 0)
    {
        yMinOffset = yMin;
    }
    else
    {
        yMinOffset = 0;
    }

    if (gridRow > 0)
    {
        gridTileInfoArray = new GridInfo * [mapRow];

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
    
    /*for (string s : cityList)
    {
        cout << s << endl;
    }*/

    generateCityInfo();

    displayMap();

    configLoaded = true;
}

void readConfigFiles()
{
    cout << "Please enter config file name" << endl;
    //TODO : clear all necessary data containers first
    string filename;

    cin >> filename;

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
      

            if (find(cityList.begin(), cityList.end(), name) == cityList.end())
            {
                cityList.push_back(name);
                idList.push_back(id);
            }

            gridTileInfoArray[x - xMinOffset][y - yMinOffset].xPos = x;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].yPos = y;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].cityID = id;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].cityName = name;
        }
        cout << endl;

        for (int i = 0; i < cityList.size(); i++)
        {
            CityInfo* info = new CityInfo();
            info->name = cityList[i];
            info->id = idList[i];
            cityInfoList.push_back(info);

            cout << info->name << endl;
            cout << info->id << endl;
        }
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

            gridTileInfoArray[x - xMinOffset][y - yMinOffset].xPos = x;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].yPos = y;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].cc = cc;
        }
        cout << endl;
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

            gridTileInfoArray[x - xMinOffset][y - yMinOffset].xPos = x;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].yPos = y;
            gridTileInfoArray[x - xMinOffset][y - yMinOffset].ap = ap;
        }
        cout << endl;
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
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        if (x == 0 && y == 0)
                            continue;

                        if (i + x > xMax || j + y > yMax || i + x < xMin || j + y < yMin)
                        {
                            cout << "denied " << endl;
                            continue;
                        }

                        if (gridTileInfoArray[i + x][j + y].cityName == gridTileInfoArray[i][j].cityName) //if adjacent tile is part of current tile's city, ignore
                        {
                            continue;
                        }
                        else
                        {
                            for (CityInfo* info : cityInfoList)
                            {
                                if (info->name == gridTileInfoArray[i][j].cityName)
                                {
                                    if (find(info->adjacentGrids.begin(), info->adjacentGrids.end(), &gridTileInfoArray[i + x][j + y]) != info->adjacentGrids.end())
                                    {
                                        continue; //skip if pointer to tile already exists in the vector
                                    }
                                    else
                                        info->adjacentGrids.push_back(&gridTileInfoArray[i + x][j + y]); //push pointer to adjacent tile into vector
                                
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //for (CityInfo* info : cityInfoList)
    //{
    //    cout << info->name << endl;
    //    for (GridInfo* grid : info->cityGrids)
    //    {
    //        cout << grid->xPos << "," << grid->yPos << endl;
    //    }
    //}
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

    displayingMap = true;
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
                    int temp = j + yMinOffset - 2;

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
                    int temp2 = i + xMinOffset - 2;

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

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].cityName != "")
            {
                gridArray[j + 2][i + 2] = to_string(gridTileInfoArray[i][j].cityID) + "  ";
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

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].cc != NULL)
            {
                string temp;

                if (gridTileInfoArray[i][j].cc <= 0 || gridTileInfoArray[i][j].cc < 10)
                {
                    temp = "0";
                }

                if (gridTileInfoArray[i][j].cc >= 10 && gridTileInfoArray[i][j].cc < 100)
                {
                    temp = to_string(gridTileInfoArray[i][j].cc)[0];
                }

                gridArray[j + 2][i + 2] = temp + "  ";
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

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].cc != NULL)
            {
                string temp;

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

                gridArray[j + 2][i + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void displayAPMapIndex()
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].ap != NULL)
            {
                string temp;

                if (gridTileInfoArray[i][j].ap <= 0 || gridTileInfoArray[i][j].ap < 10)
                {
                    temp = "0";
                }

                if (gridTileInfoArray[i][j].ap >= 10 && gridTileInfoArray[i][j].ap < 100)
                {
                    temp = to_string(gridTileInfoArray[i][j].ap)[0];
                }

                gridArray[j + 2][i + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void displayAPMapLMH()
{
    if (!configLoaded)
    {
        cout << "Config files not yet loaded, please read in a configuration file first!" << endl;
        return;
    }

    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (gridTileInfoArray[i][j].ap != NULL)
            {
                string temp;

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

                gridArray[j + 2][i + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void displayWeatherReport()
{
    cout << endl;
    cout << "[ Weather Forecast Summary Report ]" << endl;
    cout << "===================================" << endl;

    /*for (CityInfo city : cityInfoList)
    {
        int id;
        float aveCC;
        float aveAP;
        float rainProb;

        for (int i = 0; i < mapRow; i++)
        {
            for (int j = 0; j < mapCol; j++)
            {
                if (gridTileInfoArray[i][j].ap != NULL)
                {
                    if (gridTileInfoArray[i][j].cityName == s)
                    {
                        id = gridTileInfoArray[i][j].cityID;
                    }
                }
            }
        }
        
    }*/
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



