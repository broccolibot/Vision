#pragma once

#include <histogram.hpp>
#include <broccoli_detector.hpp>
#include <opencv2/opencv.hpp>

struct PipelineOutput {
    unsigned short broccoli_depth;
    bool broccoli_was_found = false;
    ScoredHead largest_head;
    BroccoliDetectorOutput detector_output;
};

class Pipeline {
    public:
        Pipeline(BroccoliDetector detector, unsigned short hist_min, unsigned short hist_max, float percentile);
        BroccoliDetector detector;
        float percentile;
        PipelineOutput next_frameset(cv::Mat& bgr_frame, cv::Mat& depth_frame);
    private:
        Histogram<unsigned short> histogram;
};
