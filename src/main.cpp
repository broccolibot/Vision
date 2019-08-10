#include <broccoli_detector.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <pipeline.hpp>
#include <pipeline_ui.hpp>

int main() {
    cv::Mat bgr_frame =
        cv::imread("/home/duncan/Media/Images/datasets/Rec1/bgr-1532.png");
    cv::Mat depth_frame =
        cv::imread("/home/duncan/Media/Images/datasets/Rec1/dep-1532.png", cv::IMREAD_ANYDEPTH | cv::IMREAD_GRAYSCALE);
    assert(depth_frame.type() == CV_16UC1);

    BroccoliDetector detector;
    detector.area_threshold = 2;
    detector.laplacian_threshold = 30;
    detector.morph_size = 5;
    detector.record_hsv_thresh = true;
    detector.record_laplacian = false;
    detector.record_morph_blob = false;

    auto pipeline = std::make_shared<Pipeline>(std::move(detector), 10, 4000, 0.1);
    PipelineUI ui(pipeline);

    while (cv::waitKey(20) != 'q') {
        auto output = pipeline->next_frameset(bgr_frame, depth_frame);
        ui.next_frame(bgr_frame, output);
    }
}
