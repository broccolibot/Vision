#include <broccoli_detector.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <pipeline.hpp>
#include <pipeline_ui.hpp>

int main(int argc, char** argv) {
    char* config_dir = (char*)"config.yml";
    if (argc > 1) config_dir = argv[1];

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <config.yml> <dataset dir>" << std::endl;
        return -1;
    }
    char* dataset_dir = argv[2];

    int frame_idx = 0;
    if (argc >= 4) {
        frame_idx = atoi(argv[3]);
    }

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
            fs["min_hsv"] >> detector.min_hsv;
            fs["max_hsv"] >> detector.max_hsv;
            fs["percentile"] >> percentile;
            fs.release();
        }
    }

    auto pipeline = std::make_shared<Pipeline>(std::move(detector), hist_min, hist_max, percentile);

    PipelineUI ui(pipeline);

    bool oneshot = true;
    bool paused = false;
    bool running = true;
    cv::Mat bgr_frame, depth_frame;
    while (running) {
        if (!paused || oneshot) {
            std::string bgr_frame_dir = std::string(dataset_dir) + "bgr-" + std::to_string(frame_idx) + ".png";
            std::string depth_frame_dir = std::string(dataset_dir) + "dep-" + std::to_string(frame_idx) + ".png";
            //std::cout << "BGR: " << bgr_frame_dir << std::endl;
            //std::cout << "DEPTH: " << depth_frame_dir << std::endl;
            //std::cout << "Frame: " << frame_idx << std::endl;
            bgr_frame = cv::imread(bgr_frame_dir);
            depth_frame = cv::imread(depth_frame_dir, cv::IMREAD_GRAYSCALE | cv::IMREAD_ANYDEPTH);
            if (!oneshot) frame_idx++;
            oneshot = false;
        }
        auto output = pipeline->next_frameset(bgr_frame, depth_frame);
        ui.next_frame(bgr_frame, output);
        switch (cv::waitKey(20)) {
            case ' ':
                paused = !paused;
                break;
            case 'h': 
                if (frame_idx > 0) frame_idx--; 
                oneshot = true;
                break;
            case 'l': 
                frame_idx++; 
                oneshot = true;
                break;
            case 'q':
                running = false;
                break;
        }
    }

    {
        cv::FileStorage fs(config_dir, cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE);
        if (fs.isOpened()) {
            fs << "area_threshold" << pipeline->detector.area_threshold;
            fs << "laplacian_threshold" << pipeline->detector.laplacian_threshold;
            fs << "morph_size" << pipeline->detector.morph_size;
            fs << "hist_min" << hist_min;
            fs << "hist_max" << hist_max;
            fs << "min_hsv" << pipeline->detector.min_hsv;
            fs << "max_hsv" << pipeline->detector.max_hsv;
            fs << "percentile" << pipeline->percentile;
            fs.release();
        }
    }
}
