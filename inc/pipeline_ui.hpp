#pragma once

#include <opencv2/opencv.hpp>
#include <pipeline.hpp>

class PipelineUI {
    public:
        PipelineUI(std::shared_ptr<Pipeline> pipeline);
        void next_frame(cv::Mat& bgr_frame, PipelineOutput& pipeline_output);
    private:
        std::shared_ptr<Pipeline> pipeline;
};
