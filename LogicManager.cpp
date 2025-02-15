#include "LogicManager.h"

LogicManager::LogicManager(): grid(GRID_SIZE, std::vector<int>(GRID_SIZE, 0))
{

}



void LogicManager::resetGrid() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), 0);
    }
}

void LogicManager::processSpatialData(const std::vector<dai::SpatialLocationCalculatorData::SpatialLocation>& spatialData, int frameWidth, int frameHeight) {
    resetGrid();
    for (const auto& data : spatialData) {
        auto coords = data.spatialCoordinates;
        float distance = std::sqrt(coords.x * coords.x + coords.y * coords.y + coords.z * coords.z);

        int col = static_cast<int>(data.config.roi.topLeft().x * GRID_SIZE);
        int row = static_cast<int>(data.config.roi.topLeft().y * GRID_SIZE);

        if (distance / 1000.0f < 1.0f) {
            grid[row][col]++;
        }
    }
}

std::string LogicManager::decideAction() const {
    int leftDanger = 0, rightDanger = 0, topDanger = 0, bottomDanger = 0;
    int leftSafe = 0, rightSafe = 0, topSafe = 0, bottomSafe = 0;
    int total = 0;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            total += grid[i][j];
            if (i < GRID_SIZE - 1) topDanger += grid[i][j];
            if (i > 0) bottomDanger += grid[i][j];
            if (j < GRID_SIZE - 1) leftDanger += grid[i][j];
            if (j > 0) rightDanger += grid[i][j];
            if (i == 0) topSafe += grid[i][j];
            if (i == GRID_SIZE - 1) bottomSafe += grid[i][j];
            if (j == 0) leftSafe += grid[i][j];
            if (j == GRID_SIZE - 1) rightSafe += grid[i][j];
        }
    }

    if (total == 0) return "Không có vật cản, tiếp tục bay";
    if (leftDanger > 0 && rightSafe == 0) return "Né phải";
    if (rightDanger > 0 && leftSafe == 0) return "Né trái";
    if (topDanger > 0 && bottomSafe == 0) return "Bay xuống";
    if (bottomDanger > 0 && topSafe == 0) return "Bay lên";
    return "Lùi lại";
}
