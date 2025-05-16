#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cstring>

const int totalRow = 6, totalCol = 6;

void drawVertLines(bool arr[][totalCol], int totalRow, int totalCol)
{
    for (int row = 0; row < totalRow; row++)
    {
        for (int col = 0; col < totalCol; col++)
        {
            if (col % 2 == 0)
            {
                arr[row][col] = true;
            }
            else
            {
                arr[row][col] = false;
            }
        }
    }
}

void drawSideLines(bool arr[][totalCol], int totalRow, int totalCol)
{
    for (int row = 0; row < totalRow; row++)
    {
        for (int col = 0; col < totalCol; col++)
        {
            if (row == 0 || row == totalRow - 1)
            {
                arr[row][col] = true;
            }
            else
            {
                arr[row][col] = false;
            }
        }
    }
}

int main()
{
    int count = 0;

    bool arr[totalRow][totalCol];
    bool newArr[totalRow][totalCol];

    static const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    // drawVertLines(arr, totalRow, totalCol);
    drawSideLines(arr, totalRow, totalCol);

    while (true)
    {
        for (int row = 0; row < totalRow; row++)
        {
            for (int col = 0; col < totalCol; col++)
            {
                if (arr[row][col])
                {
                    std::cout << "■";
                }
                else
                {
                    std::cout << "□";
                }
                std::cout << " ";
            }
            std::cout << "\n";
        }

        for (int row = 0; row < totalRow; row++)
        {
            for (int col = 0; col < totalCol; col++)
            {
                count = 0;

                for (auto dir : directions)
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

        if (memcmp(arr, newArr, sizeof(arr)) == 0)
        {
            return 0;
        }

        for (int row = 0; row < totalRow; row++)
        {
            std::copy(newArr[row], newArr[row] + totalCol, arr[row]);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for (int termRow = 0; termRow < totalRow; termRow++)
        {
            std::cout << "\033[F";
        }
    }

    return 0;
}