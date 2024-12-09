#ifndef LOGIC_MANAGER_HPP
#define LOGIC_MANAGER_HPP

#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include "depthai/depthai.hpp"

class LogicManager {
private:
    static constexpr int GRID_SIZE = 5;
    std::vector<std::vector<int>> grid;

    void resetGrid();

public:
    LogicManager();
    void processSpatialData(const std::vector<dai::SpatialLocationCalculatorData::SpatialLocation>& spatialData, int frameWidth, int frameHeight);
    std::string decideAction() const;
};

#endif
