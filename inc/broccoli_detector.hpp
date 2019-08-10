#pragma once
#include <opencv2/opencv.hpp>

struct ScoredHead {
    cv::Rect2i bounds;
    unsigned long area = 0;
    cv::Mat mask;
};

struct BroccoliDetectorOutput {
    std::vector<ScoredHead> heads;
    cv::Mat hsv_thresh_out;
    cv::Mat laplacian_out;
    cv::Mat morph_blob_out;
};

struct BroccoliDetectorSettings {
    // Tuning parameters
    cv::Vec3i min_hsv = cv::Vec3i(0, 0, 0);
    cv::Vec3i max_hsv = cv::Vec3i(255, 255, 255);
    int laplacian_threshold = 0;
    int morph_size = 0;
    unsigned long area_threshold = 1;

    // Output record settings
    bool record_hsv_thresh = false;
    bool record_laplacian = false;
    bool record_morph_blob = false;
};

BroccoliDetectorOutput detect_broccoli(BroccoliDetectorSettings& settings, cv::Mat& frame);
