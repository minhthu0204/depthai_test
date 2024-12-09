#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include "PipelineManager.h"
#include "LogicManager.h"
#include <opencv2/opencv.hpp>

class DisplayManager {
private:
    PipelineManager pipelineManager;
    LogicManager logicManager;

    dai::Device device;
    std::shared_ptr<dai::DataOutputQueue> depthQueue;
    std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue;

    void processFrame();
    void drawROIs(cv::Mat& frame, const std::vector<dai::SpatialLocationCalculatorData::SpatialLocation>& spatialData);

public:
    DisplayManager();
    void run();
};

#endif
