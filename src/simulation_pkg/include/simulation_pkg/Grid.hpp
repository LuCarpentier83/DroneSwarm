#pragma once
#include <vector>
#include <iostream>

struct Cell {
    int x;
    int y;
    bool isBlocked;
};

class Grid
{
  public:
    Grid(const int height, const int width) : height_(height), width_(width)
    {
        cells_.reserve(width * height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                cells_.push_back(Cell{x, y, false});
            }
        }
    };
    ~Grid() = default;

    static Grid generateRandomGrid(int length, int width, int n_obstacle);

    [[nodiscard]] bool isFree(const int x, const int y) const
    {
        return cells_[y * width_ + x].isBlocked == false;
    }
    void setCell(int x, int y, const bool isBlocked)
    {
        cells_[y * width_ + x].isBlocked = isBlocked;
    }

    [[nodiscard]] bool isFree(const Cell &c) const
    {
        if (isValid(c.x, c.y)) {
            return isFree(c.x, c.y);
        } else {
            std::cerr << "[Grid.isFree()]Tried to access to invalid position x=%.2d y=%.2d" << c.x << " " << c.y;
            return false;
        }
    }

    void setCell(const Cell &c, const bool isBlocked)
    {
        if (isValid(c.x, c.y)) {
            setCell(c.x, c.y, isBlocked);
        } else {
            std::cerr << "[Grid.setCell()]Tried to access to invalid position x=%.2d y=%.2d" << c.x << " " << c.y;
        }
    }

    [[nodiscard]] bool isValid(const int x, const int y) const
    {
        if (x < width_ && x >= 0 && y >= 0 && y < height_)
            return true;
        return false;
    }

  private:
    int height_;
    int width_;
    std::vector<Cell> cells_;
};