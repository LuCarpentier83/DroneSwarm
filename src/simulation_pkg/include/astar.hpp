#pragma once
#include "grid.hpp"
#include <utility>
class Astar
{
  public:
    explicit Astar();

  private:
    bool isGridSolvable(const Grid &grid, const Cell start, const Cell end);
};