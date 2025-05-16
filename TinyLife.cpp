#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <csignal>
#include <string>
#include <cstdlib>
#include <vector>

int totalRow = 20, totalCol = 20, sleepMillis = 0;

std::vector<std::vector<bool>> arr;
std::vector<std::vector<bool>> newArr;

void drawSideLines()
{
    for (int row = 1; row < totalRow - 1; row++)
    {
        std::fill(arr[row].begin(), arr[row].end(), false);
    }
    std::fill(arr[0].begin(), arr[0].end(), true);
    std::fill(arr[totalRow - 1].begin(), arr[totalRow - 1].end(), true);
}

void drawRandom()
{
    for (int row = 0; row < totalRow; row++)
    {
        for (int col = 0; col < totalCol; col++)
        {
            arr[row][col] = (std::rand() % 2) == 0;
        }
    }
}

void outputGrid()
{
    for (int row = 0; row < totalRow; row++)
    {
        std::string line;
        line.reserve(totalCol * 2);
        for (int col = 0; col < totalCol; ++col)
        {
            line += (arr[row][col] ? "■" : ".");
            line += ' ';
        }
        std::cout << line << "\n";
    }
}

void showCursorAndExit(int signum)
{
    std::cout << "\033[?25h" << std::flush;
    std::exit(signum);
}

int main(int argc, char *argv[])
{

    if (argc == 1) // no arguments provided
    {
        totalRow = 20, totalCol = 20, sleepMillis = 250;
    }
    else if (argc == 4)
    {
        totalRow = std::atoi(argv[1]);
        totalCol = std::atoi(argv[2]);
        sleepMillis = std::atoi(argv[3]);

        if (totalRow <= 2 || totalCol <= 2 || sleepMillis < 0)
        {
            std::cerr << "Invalid arguments: totalRow and totalCol must be > 2, sleep_milliseconds >= 0\n";
            return 1;
        }
        }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <totalRow> <totalCol> <sleep_milliseconds>\n";
        std::cerr << "Or run without arguments to use defaults (50 50 250).\n";
        return 1;
    }
    arr.resize(totalRow, std::vector<bool>(totalCol, false));
    newArr.resize(totalRow, std::vector<bool>(totalCol, false));

    std::signal(SIGINT, showCursorAndExit);
    std::cout << "\033[?25l";

    int count = 0;
    static const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    drawRandom();

    while (true)
    {
        outputGrid();

        for (int row = 0; row < totalRow; row++)
        {
            for (int col = 0; col < totalCol; col++)
            {
                count = 0;

                for (auto &dir : directions)
                {
                    int checkRow = row + dir[0];
                    int checkCol = col + dir[1];

                    if (checkRow >= 0 && checkRow < totalRow &&
                        checkCol >= 0 && checkCol < totalCol &&
                        arr[checkRow][checkCol])
                    {
                        count++;
                    }
                }

                if ((count == 2 || count == 3) && arr[row][col])
                {
                    newArr[row][col] = true;
                }
                else if (count == 3 && !arr[row][col])
                {
                    newArr[row][col] = true;
                }
                else
                {
                    newArr[row][col] = false;
                }
            }
        }

        if (arr == newArr)
        {
            std::cout << "\033[?25h";
            return 0;
        }

        arr = newArr;

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis));

        std::cout << "\033[" << totalRow << "F" << std::flush;
    }

    std::cout << "\033[?25h";
    return 0;
}
