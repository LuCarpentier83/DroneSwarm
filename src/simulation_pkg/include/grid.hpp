#pragma once
#include <vector>

struct Cell {
    int x;
    int y;
    bool isBlocked;
};

class Grid
{
  public:
    Grid(const int height, const int width)
        : height_(height), width_(width), cells_(width * height, false) {};
    static Grid generateRandomGrid(int length, int width, int n_obstacle);

    bool isFree(int x, int y) const
    {
        return cells_[y * width_ + x].isBlocked == false;
    }
    void setCell(int x, int y, bool isBlocked)
    {
        cells_[y * width_ + x].isBlocked = isBlocked;
    }

    bool isFree(const Cell &c) const
    {
        return isFree(c.x, c.y);
    }

    void setCell(const Cell &c, bool isBlocked)
    {
        setCell(c.x, c.y, isBlocked);
    }

  private:
    int height_;
    int width_;
    std::vector<bool> cells_;
};