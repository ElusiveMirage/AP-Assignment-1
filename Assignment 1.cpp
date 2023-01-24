// Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Assignment 1.h"

int main()
{
    while (!configLoaded)
    {
        displayingMap = false;

        cout << "Welcome to the system or whatever the fuck" << endl;
        cout << "Please enter config file name" << endl;

        cin >> fileToLoad;

        readConfigFiles(fileToLoad);
        
        initConfig();

        displayMap();
    }

    while (configLoaded) {

        if (!displayingMap)
        {
            cout << endl;
            cout << "[ Welcome to Virtual Solutions Weather Information Procesing System ]" << endl;
            cout << "=====================================================================" << endl;
            cout << "Please select an option to proceed" << endl;
            cout << "1) Display city map" << endl;
            cout << "2) Display cloud coverage map (cloudiness index)" << endl;
            cout << "3) Display cloud coverage map (LMH)" << endl;
            cout << "4) Display atmospheric pressure map (pressure index)" << endl;
            cout << "5) Display atmospheric pressure map (LMH)" << endl;
            cout << "6) Display weather forecast summary report" << endl;
            cout << "7) Exit application" << endl;

            cin >> selectedOption;

            switch (selectedOption)
            {
            case 1:
                cout << "Display city map" << endl;
                displayCityMap();
                break;
            case 2:
                cout << "Display cloud coverage map (cloudiness index)" << endl;
                displayCCMapIndex();
                break;
            case 3:
                cout << "Display cloud coverage map (LMH)" << endl;
                displayCCMapLMH();
                break;
            case 4:
                cout << "Display atmospheric pressure map (pressure index)" << endl;
                displayAPMapIndex();
                break;
            case 5:
                cout << "Display atmospheric pressure map (LMH)" << endl;
                displayAPMapLMH();
                break;
            case 6:
                cout << "Display weather forecast summary report" << endl;
                break;
            case 7:
                cout << "Exit application" << endl;
                Quit();
                break;
            default:
                break;
            }
        }
        else if (displayingMap)
        {
            char ch;

            cout << "Press Enter to return to main menu" << endl;

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
    gridRow = ((xMax - xMin) + 1) + 3;
    gridCol = ((yMax - yMin) + 1) + 3;

    mapRow = (xMax - xMin) + 1;
    mapCol = (yMax - yMin) + 1;

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
        mapTileInfoArray = new GridInfo * [mapRow];

        for (int i = 0; i < mapRow; i++)
        { 
            mapTileInfoArray[i] = new GridInfo[mapCol];
        }

        for (int i = 0; i < mapRow; i++)
        {
            for (int j = 0; j < mapCol; j++)
            {
                GridInfo* temp = new GridInfo();

                mapTileInfoArray[i][j] = *temp;
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
    
    configLoaded = true;
}

void readConfigFiles(string filename)
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
            cout << aLine << endl;

            if (aLine == "") continue;

            if (aLine[0] != '/')
            {
                configLines.push_back(aLine);

            }
        }
        cout << endl;
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

        while (getline(inputFile, aLine))
        {
            cout << aLine << endl;

            if (aLine == "") continue;

            int x = stoi(aLine.substr(aLine.find('[') + 1, aLine.find(',')));
            int y = stoi(aLine.substr(aLine.find(',') + 2, aLine.find(']')));
            int id = stoi(aLine.substr(aLine.find('-') + 1, aLine.find_last_of('-')));
            string name = aLine.substr(aLine.find_last_of('-') + 1);

            mapTileInfoArray[x - xMinOffset][y - yMinOffset].xPos = x;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].yPos = y;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].cityID = id;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].cityName = name;
        }
        cout << endl;
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
            cout << aLine << endl;

            if (aLine == "") continue;

            int x = stoi(aLine.substr(aLine.find('[') + 1, aLine.find(',')));
            int y = stoi(aLine.substr(aLine.find(',') + 2, aLine.find(']')));
            int cc = stoi(aLine.substr(aLine.find('-') + 1));

            mapTileInfoArray[x - xMinOffset][y - yMinOffset].xPos = x;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].yPos = y;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].cc = cc;
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
            cout << aLine << endl;

            if (aLine == "") continue;

            int x = stoi(aLine.substr(aLine.find('[') + 1, aLine.find(',')));
            int y = stoi(aLine.substr(aLine.find(',') + 2, aLine.find(']')));
            int ap = stoi(aLine.substr(aLine.find('-') + 1));

            mapTileInfoArray[x - xMinOffset][y - yMinOffset].xPos = x;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].yPos = y;
            mapTileInfoArray[x - xMinOffset][y - yMinOffset].ap = ap;
        }
        cout << endl;
    }
    else
    {
        cout << "Atmospheric pressure file does not exist!" << endl;
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
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (mapTileInfoArray[i][j].cityName != "")
            {
                gridArray[i + 2][j + 2] = to_string(mapTileInfoArray[i][j].cityID) + "  ";
            }
        }
    }

    showGrid();
}

void displayCCMapIndex()
{
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (mapTileInfoArray[i][j].cc != NULL)
            {
                string temp;

                if (mapTileInfoArray[i][j].cc <= 0 || mapTileInfoArray[i][j].cc < 10)
                {
                    temp = "0";
                }

                if (mapTileInfoArray[i][j].cc >= 10 && mapTileInfoArray[i][j].cc < 100)
                {
                    temp = to_string(mapTileInfoArray[i][j].cc)[0];
                }

                gridArray[i + 2][j + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void displayCCMapLMH()
{
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (mapTileInfoArray[i][j].cc != NULL)
            {
                string temp;

                if (mapTileInfoArray[i][j].cc >= 0 && mapTileInfoArray[i][j].cc < 35)
                {
                    temp = "L";
                }
                else if (mapTileInfoArray[i][j].cc >= 35 && mapTileInfoArray[i][j].cc < 65)
                {
                    temp = "M";
                }
                else if (mapTileInfoArray[i][j].cc >= 65 && mapTileInfoArray[i][j].cc < 100)
                {
                    temp = "H";
                }

                gridArray[i + 2][j + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void displayAPMapIndex()
{
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (mapTileInfoArray[i][j].ap != NULL)
            {
                string temp;

                if (mapTileInfoArray[i][j].ap <= 0 || mapTileInfoArray[i][j].ap < 10)
                {
                    temp = "0";
                }

                if (mapTileInfoArray[i][j].ap >= 10 && mapTileInfoArray[i][j].ap < 100)
                {
                    temp = to_string(mapTileInfoArray[i][j].ap)[0];
                }

                gridArray[i + 2][j + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void displayAPMapLMH()
{
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapCol; j++)
        {
            if (mapTileInfoArray[i][j].ap != NULL)
            {
                string temp;

                if (mapTileInfoArray[i][j].ap >= 0 && mapTileInfoArray[i][j].ap < 35)
                {
                    temp = "L";
                }
                else if (mapTileInfoArray[i][j].ap >= 35 && mapTileInfoArray[i][j].ap < 65)
                {
                    temp = "M";
                }
                else if (mapTileInfoArray[i][j].ap >= 65 && mapTileInfoArray[i][j].ap < 100)
                {
                    temp = "H";
                }

                gridArray[i + 2][j + 2] = temp + "  ";
            }
        }
    }

    showGrid();
}

void deallocMemory()
{
    if (mapCol <= 0)
        return;

    for (int i = 0; i < mapRow; i++)
    {
        cout << "pointer array at memory address" << &mapTileInfoArray[i] << "deleted" << endl;
        delete[] mapTileInfoArray[i];
    }
        
    delete[] mapTileInfoArray;

    if (gridCol <= 0)
        return;

    for (int i = 0; i < gridRow; i++)
    {
        delete[] gridArray[i];
        cout << "array deleted" << endl;
    }

    delete[] gridArray;
}

void Quit()
{
    deallocMemory();

    exit(0);
}

