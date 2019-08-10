#include <broccoli_detector.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <histogram.hpp>

int main() {
    cv::Mat bgr_frame =
        cv::imread("/home/duncan/Media/Images/datasets/Rec1/bgr-1532.png");
    cv::Mat depth_frame =
        cv::imread("/home/duncan/Media/Images/datasets/Rec1/dep-1532.png", cv::IMREAD_ANYDEPTH | cv::IMREAD_GRAYSCALE);
    assert(depth_frame.type() == CV_16UC1);
    cv::Mat output_image;
    bgr_frame.copyTo(output_image);

    BroccoliDetectorSettings settings;
    settings.area_threshold = 2;
    settings.laplacian_threshold = 30;
    settings.morph_size = 5;
    settings.record_hsv_thresh = false;
    settings.record_laplacian = false;
    settings.record_morph_blob = false;
    BroccoliDetectorOutput output = detect_broccoli(settings, bgr_frame);

    ScoredHead largest;
    for (auto &head : output.heads) {
        cv::rectangle(output_image, head.bounds, cv::Scalar(0, 0, 255), 2);
        if (head.area > largest.area) {
            largest = head;
        }
    }

    auto histogram = Histogram<unsigned short>(10, 4000);
    cv::Mat largest_depth;
    depth_frame(largest.bounds).copyTo(largest_depth, largest.mask);
    histogram.insert_image(largest_depth);

    cv::rectangle(output_image, largest.bounds, cv::Scalar(0, 255, 0), 2);
    cv::imshow("Depth", largest_depth);
    cv::imshow("Largest mask", largest.mask);
    cv::imshow("Output", output_image);
    std::cout << "10th PERCENTILE: " << histogram.take_percentile(0.1) << std::endl;
    // cv::imshow("morph", output.morph_blob_out);
    while (cv::waitKey() != 'q');
}
