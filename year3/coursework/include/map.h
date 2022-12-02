//
// Created by Alexey Klimov on 13.02.2022.
//

#pragma once

#include "drawer.h"
#include "optimizer.h"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/core/types.hpp>

#include <atomic>
#include <shared_mutex>
#include <vector>
#include <map>


struct Config;
class Frame;
class MapPoint;


class Map {
    friend class Drawer;
    friend class Tracking;

public:

    Map(const Config& config, const Optimizer& optimizer);

    ~Map();

    void addFrame(StereoFrame* frame);

    MapPoint* createMapPoint(const Eigen::Vector3d& position);

    [[nodiscard]] size_t mapPointsSize() const;

private:
    static std::vector<Eigen::Isometry3d> parseGroundTruth(const Config& config);

    static void run(Map* map);

    Optimizer mOptimizer;
    const Config& mConfig;

    std::vector<Frame*> mAllFrames;
    std::map<size_t, StereoFrame*> mKeyFrames;
    std::vector<MapPoint*> mMapPoints;

    mutable std::shared_mutex mMapMutex;
    std::thread mThread;
    std::atomic<bool> mIsFinish = false;

    Drawer mDrawer;
    std::vector<Eigen::Isometry3d> mGTPoses; // Ground Truth
};
