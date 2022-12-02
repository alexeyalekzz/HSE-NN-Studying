//
// Created by Alexey Klimov on 12.06.2022.
//
#pragma once

#include <Eigen/Core>

#include <map>

#define USE_CERES


struct Config;
class StereoFrame;
class MapPoint;

class Optimizer {
public:
    static constexpr int SLIDING_WINDOW_SIZE = 1000;

    Optimizer(const Config& config, const std::vector<float>& calib_data);

    void localBA(std::map<size_t, StereoFrame*>& kframes);

private:
    Eigen::Matrix3d K;
    const Config& mConfig;
};
