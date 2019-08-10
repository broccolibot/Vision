#pragma once

#include <opencv2/core.hpp>
#include <cstring>

/* TODO: Arithmetic check on template, unit tests */

template <class T>
class Histogram {
    public:
        Histogram(T min_input, T max_input);
        void insert(T input);
        void insert_image(cv::Mat& input);
        T take_percentile(float percentile);
        void print();
        void clear();
        ~Histogram();

        // Safe copy, move
        Histogram(const Histogram<T>& hist); //Copy constructor
        Histogram(Histogram<T>&& src) noexcept; //Copy constructor [C++11]
        Histogram<T>& operator=(Histogram<T> rhs); //Copy on assignment
        Histogram<T>& operator=(Histogram<T>&& rhs) noexcept; //Copy on assignment [C++11]
        void swap(Histogram<T>& rhs); //Swap
    private:
        T min_input, max_input;
        unsigned long accumulator = 0;
        unsigned int* bins = nullptr;
        unsigned int bin_count();
        size_t array_size();
};

// Constructor
template <class T>
Histogram<T>::Histogram(T min_input, T max_input) : min_input(min_input), max_input(max_input) {
    if (min_input > max_input) throw std::logic_error("Range has negative size.");
    bins = new unsigned int[bin_count()]();
}

// Insertion methods
template <class T>
void Histogram<T>::insert(T input) {
    if (input >= min_input && input <= max_input) {
        bins[input - min_input]++;
        accumulator++;
    }
}

template <>
void Histogram<unsigned short>::insert_image(cv::Mat& input);

// Utility functions
template <class T>
T Histogram<T>::take_percentile(float percentile) {
    assert(percentile >= 0.0 && percentile <= 1.0);
    unsigned long accumulator_end = accumulator * percentile; 
    unsigned long accumulator_local = 0;
    unsigned int index = 0;
    while (accumulator_local < accumulator_end) {
        accumulator_local += bins[index];
        if (++index >= bin_count()) return max_input;
    }
    return index + min_input;
}

template <class T>
void Histogram<T>::clear() {
    accumulator = 0;
    memset(bins, 0, array_size());
}

/*
template <>
void Histogram<unsigned short>::print() {
    for (int i = 0; i < bin_count(); i++) {
        printf("%i: %u\n", i + min_input, bins[i]);
    }
}
*/

// Internal functions
template <class T>
unsigned int Histogram<T>::bin_count() {
    return (max_input - min_input) + 1; //Include last and first
}

template <class T>
size_t Histogram<T>::array_size() {
    return sizeof(*bins) * bin_count();
}

// Destructor
template <class T>
Histogram<T>::~Histogram() {
    delete[] bins;
}

// Copy constructors
template <class T>
Histogram<T>::Histogram(const Histogram<T>& hist) {
    accumulator = hist.accumulator;
    max_input = hist.max_input;
    min_input = hist.min_input;
    bins = new unsigned int[this->bin_count()]();
    memcpy(bins, hist.bins, this->array_size());
}

template <class T>
Histogram<T>::Histogram(Histogram<T>&& src) noexcept {
    src.swap(*this);
}

// Assignment operators
template <class T>
Histogram<T>& Histogram<T>::operator=(Histogram<T> rhs) {
    rhs.swap(*this);
    return *this;
}

template <class T>
Histogram<T>& Histogram<T>::operator=(Histogram<T>&& rhs) noexcept {
    rhs.swap(*this);
    return *this;
}

// Swapping
template <class T>
void Histogram<T>::swap(Histogram<T>& rhs) {
    std::swap(this->bins, rhs.bins);
    std::swap(this->accumulator, rhs.accumulator);
    std::swap(this->min_input, rhs.min_input);
    std::swap(this->max_input, rhs.max_input);
}
