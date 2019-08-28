#include <broccoli_detector.hpp>

BroccoliDetectorOutput BroccoliDetector::detect_broccoli(cv::Mat& input_frame) {
    BroccoliDetectorOutput output;

    cv::Mat color_hsv;
    cv::cvtColor(input_frame, color_hsv, cv::COLOR_RGB2HSV);

    cv::Mat hsv_mask;
    cv::inRange(color_hsv, min_hsv, max_hsv, hsv_mask);
    color_hsv.release();
    if (record_hsv_thresh) hsv_mask.copyTo(output.hsv_thresh_out);

    cv::Mat color_split[3];
    cv::split(input_frame, color_split);
    color_split[0].release();
    color_split[2].release();

    cv::Mat laplacian;
    cv::Laplacian(color_split[1], laplacian, CV_8UC1);
    color_split[1].release();

    cv::Mat laplacian_confirmed;
    cv::inRange(laplacian, cv::Scalar(laplacian_threshold), cv::Scalar(255), laplacian_confirmed);
    laplacian.release();
    if (record_laplacian) laplacian_confirmed.copyTo(output.laplacian_out);

    cv::Mat lap_conf_mask_combined;
    cv::bitwise_and(laplacian_confirmed, hsv_mask, lap_conf_mask_combined);
    hsv_mask.release();
    laplacian_confirmed.release();

    cv::Mat morph_blob;
    //TODO: Maybe persist this morph element?
    cv::Mat morph_element = getStructuringElement (0, cv::Size (2 * morph_size + 1, 2 * morph_size + 1));
    cv::morphologyEx(lap_conf_mask_combined, morph_blob, cv::MORPH_CLOSE, morph_element);
    cv::morphologyEx(morph_blob, morph_blob, cv::MORPH_OPEN, morph_element);
    lap_conf_mask_combined.release();
    if (record_morph_blob) morph_blob.copyTo(output.morph_blob_out);

    cv::Mat canny_edges;
    cv::Canny(morph_blob, canny_edges, 128, 128);

	std::vector<std::vector<cv::Point>> contours;
    cv::findContours(canny_edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);
    canny_edges.release();

    std::vector<ScoredHead> heads;

    // "Close" each contour in order to make sure it has an area.
    for (auto& contour : contours) {
        contour[contour.size() - 1] = contour[0];
        cv::Rect2i bounds = boundingRect(contour);
        //unsigned long area = bounds.area();
        int area = bounds.area();
        if (area >= area_threshold * area_threshold) {
            ScoredHead head;
            head.bounds = bounds;
            head.area = area;
            morph_blob(bounds).copyTo(head.mask);
            heads.push_back(std::move(head));
        }
    }

    output.heads = std::move(heads);

    return output;
}
