#include <broccoli_detector.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <pipeline.hpp>
#include <pipeline_ui.hpp>

int main(int argc, char** argv) {
    char* config_dir = (char*)"config.yml";
    if (argc > 1) config_dir = argv[0];

    cv::Mat bgr_frame =
        cv::imread("/home/duncan/Media/Images/datasets/Rec1/bgr-1532.png");
    cv::Mat depth_frame =
        cv::imread("/home/duncan/Media/Images/datasets/Rec1/dep-1532.png", cv::IMREAD_ANYDEPTH | cv::IMREAD_GRAYSCALE);
    assert(depth_frame.type() == CV_16UC1);

    BroccoliDetector detector;
    
    // Set defaults
    detector.area_threshold = 2;
    detector.laplacian_threshold = 30;
    detector.morph_size = 5;
    detector.record_hsv_thresh = true;
    detector.record_laplacian = false;
    detector.record_morph_blob = false;
    int hist_min = 10;
    int hist_max = 4000;
    float percentile = 0.1;
    {
        cv::FileStorage fs(config_dir, cv::FileStorage::FORMAT_YAML | cv::FileStorage::READ);
        if (fs.isOpened()) {
            fs["area_threshold"] >> detector.area_threshold;
            fs["laplacian_threshold"] >> detector.laplacian_threshold;
            fs["morph_size"] >> detector.morph_size;
            fs["hist_min"] >> hist_min;
            fs["hist_max"] >> hist_max;
            fs["percentile"] >> percentile;
            fs.release();
        }
    }

    auto pipeline = std::make_shared<Pipeline>(std::move(detector), hist_min, hist_max, percentile);

    PipelineUI ui(pipeline);

    while (cv::waitKey(20) != 'q') {
        auto output = pipeline->next_frameset(bgr_frame, depth_frame);
        ui.next_frame(bgr_frame, output);
    }

    {
        cv::FileStorage fs(config_dir, cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE);
        if (fs.isOpened()) {
            fs << "area_threshold" << pipeline->detector.area_threshold;
            fs << "laplacian_threshold" << pipeline->detector.laplacian_threshold;
            fs << "morph_size" << pipeline->detector.morph_size;
            fs << "hist_min" << hist_min;
            fs << "hist_max" << hist_max;
            fs << "percentile" << pipeline->percentile;
            fs.release();
        }
    }
}
