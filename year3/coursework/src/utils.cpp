//
// Created by Alexey on 17.01.2022.
//

#include "utils.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#ifdef _WIN32
#include "windows.h"
#include "Psapi.h"
#endif


void displayPoints(cv::Mat img, const std::vector<cv::Point2f>& pts) {
    cv::cvtColor(img, img, cv::COLOR_GRAY2BGR, 3);

    for (const auto& p : pts) {
        cv::circle(img, p, 3, CV_RGB(0, 255, 0));
    }

    cv::imshow("Features viewer", img);
    cv::waitKey(1);
}

size_t getTotalRAM() {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPhys / 1024 / 1024;
#else
    return 0;
#endif
}

size_t getCurrentlyUsedRAM() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize / 1024 / 1024;
#else
    return 0;
#endif
}

void printSummary(std::pair<double, int> max, std::pair<double, int> min, double avg, double total, size_t maxRAM) {
    int seconds = (int)total;
    int minutes = seconds / 60;
    int hours = minutes / 60;

    minutes -= hours * 60;
    seconds -= hours * 60 * 60 + minutes * 60;

    std::printf("\n############################## Summary #################################\n"
        "-- max memory usage: %lluMbs\n"
        "-- max / min frame time: [%.3lf, #%d], [%.3lf, #%d]\n"
        "-- Average frame time: %lf\n"
        "-- Total time: %02d:%02d:%02d\n",

        maxRAM,
        max.first,
        max.second,
        min.first,
        min.second,
        avg,
        hours,
        minutes,
        seconds
    );
}
