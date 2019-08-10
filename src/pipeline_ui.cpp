#include <pipeline_ui.hpp>

PipelineUI::PipelineUI(std::shared_ptr<Pipeline> pipeline) : pipeline(pipeline) {
    std::string window_name = "Settings";
    cv::namedWindow(window_name, 0);
    cv::createTrackbar("H min", window_name, &pipeline->detector.min_hsv[0], 255);
    cv::createTrackbar("H max", window_name, &pipeline->detector.max_hsv[0], 255);

    cv::createTrackbar("S min", window_name, &pipeline->detector.min_hsv[1], 255);
    cv::createTrackbar("S max", window_name, &pipeline->detector.max_hsv[1], 255);

    cv::createTrackbar("V min", window_name, &pipeline->detector.min_hsv[2], 255);
    cv::createTrackbar("V max", window_name, &pipeline->detector.max_hsv[2], 255);

    cv::createTrackbar("Lap thresh", window_name, &pipeline->detector.laplacian_threshold, 255);
    cv::createTrackbar("Area thresh", window_name, &pipeline->detector.area_threshold, 2000);
}

void PipelineUI::next_frame(cv::Mat& bgr_frame, PipelineOutput& pipeline_output) {
    cv::Mat output_image;
    bgr_frame.copyTo(output_image);

    if (pipeline->detector.record_hsv_thresh)
        cv::imshow("HSV", pipeline_output.detector_output.hsv_thresh_out);

    if (pipeline->detector.record_laplacian)
        cv::imshow("Laplacian", pipeline_output.detector_output.laplacian_out);

    if (pipeline->detector.record_morph_blob)
        cv::imshow("Morph", pipeline_output.detector_output.morph_blob_out);

    cv::rectangle(output_image, pipeline_output.largest_head.bounds, cv::Scalar(0, 255, 0), 2);
    cv::imshow("Annotated", output_image);
    std::cout << "Depth: " << pipeline_output.broccoli_depth << std::endl;

}
