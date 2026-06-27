#include "simulation_pkg/OccupancyGrid.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    const int height = 20;
    const int width = 30;
    const int n_obstacle = 40;

    Grid grid = Grid::generateRandomGrid(height, width, n_obstacle);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid.isFree(x, y)) {
                std::cout << '.';
            } else {
                std::cout << '#';
            }
        }
        std::cout << '\n';
    }

    return 0;
}