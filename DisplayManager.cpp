#include "DisplayManager.h"

DisplayManager::DisplayManager()
    : device(pipelineManager.getPipeline()) {
    device.setIrLaserDotProjectorBrightness(1000);

    depthQueue = device.getOutputQueue("depth", 4, false);
    spatialCalcQueue = device.getOutputQueue("spatialData", 4, false);
}

void DisplayManager::processFrame() {
    auto inDepth = depthQueue->get<dai::ImgFrame>();
    cv::Mat depthFrame = inDepth->getFrame();

    // Normalize and apply color map
    cv::Mat depthFrameColor;
    cv::normalize(depthFrame, depthFrameColor, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    cv::applyColorMap(depthFrameColor, depthFrameColor, cv::COLORMAP_HOT);

    auto spatialData = spatialCalcQueue->get<dai::SpatialLocationCalculatorData>()->getSpatialLocations();
    logicManager.processSpatialData(spatialData, depthFrameColor.cols, depthFrameColor.rows);

    drawROIs(depthFrameColor, spatialData);

    // Display action decision
    auto action = logicManager.decideAction();
    std::cout << "Action: " << action << std::endl;

    // Show the frame
    cv::imshow("depth", depthFrameColor);
}

void DisplayManager::drawROIs(cv::Mat& frame, const std::vector<dai::SpatialLocationCalculatorData::SpatialLocation>& spatialData) {
    for (const auto& data : spatialData) {
        auto roi = data.config.roi.denormalize(frame.cols, frame.rows);
        auto coords = data.spatialCoordinates;
        float distance = std::sqrt(coords.x * coords.x + coords.y * coords.y + coords.z * coords.z);

        int xmin = static_cast<int>(roi.topLeft().x);
        int ymin = static_cast<int>(roi.topLeft().y);
        int xmax = static_cast<int>(roi.bottomRight().x);
        int ymax = static_cast<int>(roi.bottomRight().y);

        cv::Scalar color = (distance / 1000.0f < 1.0f) ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
        cv::rectangle(frame, cv::Rect(cv::Point(xmin, ymin), cv::Point(xmax, ymax)), color, 2);
    }
}

void DisplayManager::run() {
    while (true) {
        processFrame();
        if (cv::waitKey(1) == 'q') break;
    }
}
