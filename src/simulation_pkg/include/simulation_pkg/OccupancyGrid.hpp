#pragma once
#include "Grid.hpp"
#include <cstddef>
#include <vector>

class OccupancyGrid
{
  public:
    OccupancyGrid(double width, double length, double resolution);
    static std::pair<size_t, size_t> worldToGrid(double x, double y);

  private:
    double width_;
    double length_;
    double resolution_;
    Grid grid;
};