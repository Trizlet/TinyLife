#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <csignal>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

// Initialize variables and grids
static int totalRow = 20, totalCol = 20;
static const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}; // "Chart" of cell neighbor directions

static std::vector<std::vector<bool>> grid;
static std::vector<std::vector<bool>> newGrid;
static std::vector<std::vector<bool>> temp;

void drawTopBottomSides() // Func to draw top and bottom lines
{
    std::fill(grid[0].begin(), grid[0].end(), true);
    std::fill(grid[totalRow - 1].begin(), grid[totalRow - 1].end(), true);
}

void drawLeftRightSides() // Func to draw left and right lines
{
    for (int row = 0; row < totalRow; row++)
    {
        grid[row][0] = true;
        grid[row][totalCol - 1] = true;
    }
}

void drawRandom() // Func to fill the grid randomly
{
    static std::mt19937 rng{std::random_device{}()};
    static std::bernoulli_distribution coin(0.5);

    for (int row = 0; row < totalRow; row++)
        for (int col = 0; col < totalCol; col++)
            grid[row][col] = coin(rng);
}

void outputGrid() // Func to output the whole grid line by line
{
    for (int row = 0; row < totalRow; row++)
    {
        std::string line;
        line.reserve(totalCol * 2);
        for (int col = 0; col < totalCol; col++)
        {
            line += (grid[row][col] ? "#" : ".");
            line += ' ';
        }
        std::cout << line << "\n";
    }
}

void showCursorAndExit(int signum) // Func to exit safely and restore the cursor
{
    std::cout << "\033[?25h" << std::flush;
    std::exit(signum);
}

int main(int argc, char *argv[]) // MAIN
{
    int sleepMillis = 250, preset = 0;

    // Check for help flag
    if (argc > 1 && (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0))
    {
        std::cout << "Usage:\n\n"
                  << " no args (defaults: 20 20 250 0)\n"
                  << " <rows> (square)\n"
                  << " <rows> <cols>\n"
                  << " <rows> <cols> <ms>\n"
                  << " <rows> <cols> <ms> <preset>\n"
                  << " -h | --help (show this message)\n";
        return 0;
    }

    switch (argc) // Begin command arg parsing
    {
    case 1: // Defaults: 20x20, 250ms, preset=0
        break;
    case 2: // One arg: square size
        totalRow = totalCol = std::atoi(argv[1]);
        break;
    case 3: // Two args: rows and cols
        totalRow = std::atoi(argv[1]);
        totalCol = std::atoi(argv[2]);
        break;
    case 4: // Three args: rows, cols, sleep
        totalRow = std::atoi(argv[1]);
        totalCol = std::atoi(argv[2]);
        sleepMillis = std::atoi(argv[3]);
        break;
    case 5: // Four args: rows, cols, sleep, preset
        totalRow = std::atoi(argv[1]);
        totalCol = std::atoi(argv[2]);
        sleepMillis = std::atoi(argv[3]);
        preset = std::atoi(argv[4]);
        break;
    default:
        std::cerr << "error";
        return 1;
    }

    if (totalRow <= 2 || totalCol <= 2 || sleepMillis < 1 || preset < 0 || preset > 3)
    {
        std::cerr << "Invalid parameters: rows>2, cols>2, ms>=1, preset=0-3\n";
        return 1;
    }

    std::signal(SIGINT, showCursorAndExit); // Prepare Ctrl+C exit
    std::cout << "\033[?25l";               // Hide the cursor

    // Resize vectors to the desired size
    grid.resize(totalRow, std::vector<bool>(totalCol, false));
    newGrid.resize(totalRow, std::vector<bool>(totalCol, false));

    // Draw the grid as desired
    switch (preset)
    {
    case 1:
        drawTopBottomSides();
        drawLeftRightSides();
        break;
    case 2:
        drawTopBottomSides();
        break;
    case 3:
        drawLeftRightSides();
        break;
    default:
        drawRandom();
        break;
    }

    while (true)
    {
        outputGrid(); // Print the grid

        // The game of life computation.
        for (int row = 0; row < totalRow; row++)
        {
            for (int col = 0; col < totalCol; col++)
            {
                int count = 0;
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
                    newGrid[row][col] = true;
                else if (count == 3 && !grid[row][col])
                    newGrid[row][col] = true;
                else
                    newGrid[row][col] = false;
            }
        }

        // Exit if the grid is static or looping (need to implement a way to detect longer loops)
        if (grid == newGrid || temp == newGrid)
        {
            showCursorAndExit(0);
        }

        temp = grid; // Store the current grid so 2 generations later we can check if we're caught in a simple short loop

        grid = newGrid; // Switch to the new iteration before running computation again

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMillis)); // Sleep between generations to set the sim speed

        std::cout << "\033[" << totalRow << "F" << std::flush; // Move the cursor back so the previous grid can be drawn over
    }
    showCursorAndExit(1);
}