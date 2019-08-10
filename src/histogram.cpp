#include <histogram.hpp>

// TODO: Implement this for other types?
template <>
void Histogram<unsigned short>::insert_image(cv::Mat& input) {
    if (input.type() != CV_16UC1) throw std::logic_error("Matrix type must be CV_16UC1.");
    unsigned short* input_array = (unsigned short*)input.data;
    unsigned short* input_array_end = (unsigned short*)input.dataend;
    while (input_array < input_array_end) insert(*input_array++);
}

