#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <csignal>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

// Initialize rows, cols, and sleep integers & the grid vectors
int totalRow = 20, totalCol = 20, sleepMillis = 0;
std::vector<std::vector<bool>> grid;
std::vector<std::vector<bool>> newGrid;

void drawSideLines() // Draw top and bottom lines (using for debugging)
{
    for (int row = 1; row < totalRow - 1; row++)
    {
        std::fill(grid[row].begin(), grid[row].end(), false);
    }
    std::fill(grid[0].begin(), grid[0].end(), true);
    std::fill(grid[totalRow - 1].begin(), grid[totalRow - 1].end(), true);
}

void drawRandom() // Fill the grid randomly (I should change the RNG method later)
{
    for (int row = 0; row < totalRow; row++)
    {
        for (int col = 0; col < totalCol; col++)
        {
            grid[row][col] = (std::rand() % 2) == 0;
        }
    }
}

void outputGrid() // Print the grid to the terminal (requires UTF-8! could change ■ to # if not supported)
{
    for (int row = 0; row < totalRow; row++)
    {
        std::string line;
        line.reserve(totalCol * 2);
        for (int col = 0; col < totalCol; ++col)
        {
            line += (grid[row][col] ? "■" : ".");
            line += ' ';
        }
        std::cout << line << "\n";
    }
}

void showCursorAndExit(int signum) // Restore the cursor on exit
{
    std::cout << "\033[?25h" << std::flush;
    std::exit(signum);
}

int main(int argc, char *argv[])
{

    std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed the RNG

    if (argc == 1) // No arguments provided: default
    {
        totalRow = 20, totalCol = 20, sleepMillis = 250;
    }
    else if (argc == 4) // Arguments provided
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
    // Resize vectors to the desired size
    grid.resize(totalRow, std::vector<bool>(totalCol, false));
    newGrid.resize(totalRow, std::vector<bool>(totalCol, false));

    // Prepare the cursor restoration mechanism before hiding the cursor
    std::signal(SIGINT, showCursorAndExit);
    std::cout << "\033[?25l";

    int count = 0;
    static const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    drawRandom();

    while (true)
    {
        outputGrid();

        // The game of life computation. The next iteration is stored in newGrid
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
                        grid[checkRow][checkCol])
                    {
                        count++;
                    }
                }

                if ((count == 2 || count == 3) && grid[row][col])
                {
                    newGrid[row][col] = true;
                }
                else if (count == 3 && !grid[row][col])
                {
                    newGrid[row][col] = true;
                }
                else
                {
                    newGrid[row][col] = false;
                }
            }
        }

        // Exit if the grid is static (need to implement loop detection)
        if (grid == newGrid)
        {
            std::cout << "\033[?25h";
            return 0;
        }

        grid = newGrid; // Switch to the new iteration

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis)); // Sleep if desired

        std::cout << "\033[" << totalRow << "F" << std::flush; // Move the cursor back so the previous grid can be drawn over
    }

    std::cout << "\033[?25h"; // Restore cursor
    return 0;
}