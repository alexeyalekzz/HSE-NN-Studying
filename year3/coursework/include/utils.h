//
// Created by Alexey on 17.01.2022.
//

#ifndef UTILS_H_
#define UTILS_H_

#include <opencv2/core/mat.hpp>

#include <chrono>


class Timer {
    using clock = std::chrono::high_resolution_clock;

public:
    using micros = std::chrono::duration<double, std::micro>;
    using milli = std::chrono::duration<double, std::milli>;
    using seconds = std::chrono::duration<double>;
    using minutes = std::chrono::duration<double, std::ratio<60>>;

    using time_point = decltype(clock::now());

    // Start timer
    static time_point set() {
        return clock::now();
    }

    // Get duration, in milliseconds by default
    template<class T = milli>
    static T get(time_point begin) {
        return T(clock::now() - begin);
    }

    // Estimates time in function in milliseconds
    template<class F, class... Args>
    static auto funcTime(F func, Args&&... args) {
        time_point start = clock::now();
        func(std::forward<Args>(args)...);
        return milli(clock::now() - start);
    }
};

void displayPoints(cv::Mat img, const std::vector<cv::Point2f>& pts);

// returns total amount of physical memory (RAM)
size_t getTotalRAM();

// returns amount of physical memory (RAM) that current process using in MB
size_t getCurrentlyUsedRAM();

void printSummary(std::pair<double, int> max,
                  std::pair<double, int> min,
                  double avg,
                  double total,
                  size_t maxRAM);


#endif // UTILS_H_
