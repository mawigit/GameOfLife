// GameOfLife.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Timing.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define LOG(x) std::cout << x << std::endl

const int generations = 250;
const int size = 250000;
std::string firstLine;
std::string remainingLines;
bool deadOrAlive[size];
bool nextGen[size];
int cols;
int rows;
int neighbours[8];
int currentRow;

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

    int i = 0;
    for (char c : remainingLines)
    {
        if (c == (char)'x')
        {
            deadOrAlive[i] = true;
        }
        else if (c == (char)'.')
        {
            deadOrAlive[i] = false;
        }
        i++;
    }

    // Close the file when done
    inputFile.close();
}

void setColsRows()
{
    std::istringstream iss(firstLine);

    char delimiter = ',';
    if (!(iss >> cols >> delimiter >> rows))
    {
        std::cerr << "Error reading numbers." << std::endl;
    }
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
    if (currentIndex + 1 > (getRow(currentIndex) + 1) * cols - 1)
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

    // Assuming neighbours is an array or container
    for (int i : neighbours)
    {
        // Assuming deadOrAlive is a boolean array
        aliveCount += deadOrAlive[i];
    }

    return aliveCount;
}

void setCurrentStatus(int currentIndex, int aliveCount)
{

    //• Rule 1: Any dead cell with exactly three living neighbours becomes a live cell
    if (deadOrAlive[currentIndex] == false && aliveCount == 3)
    {
        nextGen[currentIndex] = true;
    }
    //• Rule 2: Any live cell with two or three living neighbours stay alive
    else if (deadOrAlive[currentIndex] == true && (aliveCount == 2 || aliveCount == 3))
    {
        nextGen[currentIndex] = true;
    }
    //• Rule 3: Any live cell with fewer than two living neighbours dies
    else if (deadOrAlive[currentIndex] == true && aliveCount < 2)
    {
        nextGen[currentIndex] = false;
    }
    //• Rule 4: Any live cell with more than three living neighbours dies
    else if (deadOrAlive[currentIndex] == true && aliveCount > 3)
    {
        nextGen[currentIndex] = false;
    }
    else
    {
        nextGen[currentIndex] = false;
    }
}

int main()
{
    Timing* timing = Timing::getInstance();

    timing->startSetup();
    // Setup code here

    readFile("step1000_in_250generations/random250_in.gol");
    setColsRows();




    timing->stopSetup();

    timing->startComputation();
    
    
    // Computation code here
    
    for (int gen = 0; gen < generations; gen++)
    {
        for (int i = 0; i < size; i++)
        {
            getNeighbours(i);
            int aliveCount = getLivingNeighbours();
            setCurrentStatus(i, aliveCount);
        }
        for (int i = 0; i < size; i++)
        {
            deadOrAlive[i] = nextGen[i];
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
