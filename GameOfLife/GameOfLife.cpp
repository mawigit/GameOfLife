// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Timing.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define LOG(x) std::cout << x << std::endl


std::string firstLine;
std::string remainingLines;
std::vector<bool> deadOrAlive;
int cols;
int rows;
int size;
int neighbours[8];

void readFile(std::string filePath)
{
    // Read file
            // Declare an input file stream object
    std::ifstream inputFile;

    // Open the file (replace "filename.txt" with the actual file name)
    inputFile.open(filePath);

    // Check if the file is opened successfully
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening the file.\n";
    }

    // Read the first line
    std::getline(inputFile, firstLine);

    // Read the rest of the lines to bool vector
    std::string line;

    while (std::getline(inputFile, line))
    {
        remainingLines += line;
    }


    for (char c : remainingLines)
    {
        if (c == (char)'x')
        {
            deadOrAlive.push_back(true);
        }
        else if (c == (char)'.')
        {
            deadOrAlive.push_back(false);
        }
    }

    // Close the file when done
    inputFile.close();
}

void setColsRowsSize()
{
    std::istringstream iss(firstLine);

    char delimiter = ',';
    if (!(iss >> cols >> delimiter >> rows))
    {
        std::cerr << "Error reading numbers." << std::endl;
    }
    size = cols * rows;

    // Output the values
    std::cout << "Columns: " << cols << std::endl;
    std::cout << "Rows: " << rows << std::endl;

}

int getRow(int index)
{
    return (index / cols);
}

int getTopNeighbour(int currentIndex)
{
    if (currentIndex - cols < 0)
    {
        return currentIndex + (rows - 1) * cols;
    }
    else
    {
        return currentIndex - cols;
    }
}

int getRightNeighbour(int currentIndex)
{
    if (currentIndex + 1 > ((getRow(currentIndex) + 1) * cols - 1))
    {
        return currentIndex - (cols - 1);
    }
    else
    {
        return currentIndex + 1;
    }
}

int getLeftNeighbour(int currentIndex)
{
    if (currentIndex - 1 < (getRow(currentIndex) * cols))
    {
        return currentIndex + (cols - 1);
    }
    else
    {
        return currentIndex - 1;
    }
}

int getBotNeighbour(int currentIndex)
{
    if (currentIndex + cols > size - 1)
    {
        return currentIndex - (rows - 1) * cols;
    }
    else
    {
        return currentIndex + cols;
    }
}

int getTopLeftNeighbour(int currentIndex)
{
    return getLeftNeighbour(getTopNeighbour(currentIndex));
}

int getTopRightNeighbour(int currentIndex)
{
    return getRightNeighbour(getTopNeighbour(currentIndex));
}

int getBotLeftNeighbour(int currentIndex)
{
    return getLeftNeighbour(getBotNeighbour(currentIndex));
}

int getBotRightNeighbour(int currentIndex)
{
    return getRightNeighbour(getBotNeighbour(currentIndex));
}

void getNeighbours(int currentIndex)
{
    neighbours[0] = getTopLeftNeighbour(currentIndex);
    neighbours[1] = getTopNeighbour(currentIndex);
    neighbours[2] = getTopRightNeighbour(currentIndex);
    neighbours[3] = getLeftNeighbour(currentIndex);
    neighbours[4] = getRightNeighbour(currentIndex);
    neighbours[5] = getBotLeftNeighbour(currentIndex);
    neighbours[6] = getBotNeighbour(currentIndex);
    neighbours[7] = getBotRightNeighbour(currentIndex);
}

int getLivingNeighbours()
{
    int aliveCount = 0;
    for (int i : neighbours)
    {
        if (deadOrAlive[i])
        {
            aliveCount++;
        }
    }
    return aliveCount;
}

void setCurrentStatus(int currentIndex, int aliveCount)
{

    //� Rule 1: Any dead cell with exactly three living neighbours becomes a live cell
    if (deadOrAlive[currentIndex] == false && aliveCount == 3)
    {
        deadOrAlive[currentIndex] = true;
    }
    //� Rule 2: Any live cell with two or three living neighbours stay alive
    if (deadOrAlive[currentIndex] == true && (aliveCount == 2 || aliveCount == 3))
    {
    }
    //� Rule 3: Any live cell with fewer than two living neighbours dies
    if (deadOrAlive[currentIndex] == true && aliveCount < 2)
    {
        deadOrAlive[currentIndex] = false;
    }
    //� Rule 4: Any live cell with more than three living neighbours dies
    if (deadOrAlive[currentIndex] == true && aliveCount > 3)
    {
        deadOrAlive[currentIndex] = false;
    }
}

int main()
{
    Timing* timing = Timing::getInstance();

    timing->startSetup();
    // Setup code here

    readFile("step1000_in_250generations/debug.gol");
    setColsRowsSize();




    timing->stopSetup();

    timing->startComputation();
    
    ;
    
    // Computation code here
    
    for (int gen = 0; gen < 1; gen++)
    {
        LOG(gen);
        for (int i = 0; i < size; i++)
        {
            getNeighbours(i);
            int aliveCount = getLivingNeighbours();
            setCurrentStatus(i, aliveCount);
        }
    }
  
    
    
    timing->stopComputation();

    timing->startFinalization();
    // Finalization code here
    /*
    * Write code to file
    */
    std::ofstream outputFile("output.txt");

    // Check if the file is open
    if (!outputFile.is_open())
    {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    // Iterate over the array and print each element to the file
    
    for (int i = 0; i < size; ++i)
    {
        if (deadOrAlive[i] == true)
        {
            if (i % cols == 0 && i != 0)
            {
                outputFile << "\n";
            }
            outputFile << 'x';
        }
        if (deadOrAlive[i] == false)
        {
            if (i % cols == 0 && i != 0)
            {
                outputFile << "\n";
            }
            outputFile << '.';
        }
    }

    // Close the file
    outputFile.close();
    
    timing->stopFinalization();

    timing->getResults();
    timing->print(true);
    delete timing;
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
