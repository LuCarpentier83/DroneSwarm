#include "simulation_pkg/OccupancyGrid.hpp"
#include <random>

Grid Grid::generateRandomGrid(int length, int width, int n_obstacle)
{
    Grid grid(length, width);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist_x(0, width - 1);
    std::uniform_int_distribution<int> dist_y(0, length - 1);

    int placed = 0;
    while (placed < n_obstacle) {
        int x = dist_x(gen);
        int y = dist_y(gen);

        if (grid.isFree(x, y)) {
            grid.setCell(x, y, true);
            placed++;
        }
    }
    return grid;
};