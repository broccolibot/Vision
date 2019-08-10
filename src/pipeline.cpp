#include <pipeline.hpp>

Pipeline::Pipeline(
        BroccoliDetector detector, 
        unsigned short hist_min, 
        unsigned short hist_max, 
        float percentile) : 
    detector(std::move(detector)), 
    percentile(percentile), 
    histogram(hist_min, hist_max) {}

PipelineOutput Pipeline::next_frameset(cv::Mat& bgr_frame, cv::Mat& depth_frame) {
    BroccoliDetectorOutput detector_output = detector.detect_broccoli(bgr_frame);
    PipelineOutput output;

    ScoredHead largest;
    bool found = false;
    for (auto &head : detector_output.heads) {
        if (head.area > largest.area) {
            largest = head;
            found = true;
        }
    }

    output.broccoli_was_found = found;
    if (!found) {
        return output;
    }
    output.largest_head = largest;
    output.detector_output = detector_output;

    cv::Mat largest_depth;
    depth_frame(largest.bounds).copyTo(largest_depth, largest.mask);

    histogram.clear();
    histogram.insert_image(largest_depth);

    output.broccoli_depth = histogram.take_percentile(percentile);

    return output;
}
