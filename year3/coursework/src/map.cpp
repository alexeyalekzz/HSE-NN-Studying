//
// Created by Alexey Klimov on 13.02.2022.
//

#include "map.h"

#include "config_reader.h"
#include "frame.h"
#include "map_point.h"
#include "optimizer.h"

#include <fstream>
#include <iostream>


std::vector<Eigen::Isometry3d> Map::parseGroundTruth(const Config& config) {
    std::ifstream fin(config.gt_path);

    if (!fin.is_open()) {
        std::cout << "-! Error reading ground truth file" << std::endl;
        return { };
    }

    std::vector<Eigen::Isometry3d> gt;
    std::string matrx_str;
    while (std::getline(fin, matrx_str, '\n')) {
        std::stringstream ss(std::move(matrx_str));
        double pose[12]{ 0 };
        for (auto& elem : pose) { ss >> elem; }

        Eigen::Matrix3d rotmat {
            { pose[0], pose[1], pose[2] },
            { pose[4], pose[5], pose[6] },
            { pose[8], pose[9], pose[10] }
        };
        Eigen::Vector3d tvec { pose[3], pose[7], pose[11] };

        Eigen::Isometry3d se3;
        se3.rotate(rotmat);
        se3.translation() = tvec;
        gt.push_back(se3);
    }
    return gt;
}

Map::Map(const Config& config, const Optimizer& optimizer)
    : mConfig(config)
    , mOptimizer(optimizer)
    , mGTPoses(parseGroundTruth(config))
    , mThread(run, this)
    , mDrawer(*this)
{}

Map::~Map() {
    mIsFinish = true;
    mThread.join();

    for (auto* f : mAllFrames) {
        delete f;
    }
    for (auto* mp : mMapPoints) {
        delete mp;
    }
}

void Map::addFrame(StereoFrame* frame) {
    static int i = 0;
    {
        std::unique_lock lock(mMapMutex);
        mAllFrames.push_back(frame);
        if (frame->isKeyFrame()) {
            mKeyFrames.insert({ frame->ID, frame });
            i++;
        }
    }
    if (i >= 2 && frame->isKeyFrame() && (mKeyFrames.size() >= Optimizer::SLIDING_WINDOW_SIZE)) {
        mOptimizer.localBA(mKeyFrames);
        i = 0;
    }
}

MapPoint* Map::createMapPoint(const Eigen::Vector3d& position) {
    std::unique_lock lock(mMapMutex);
    mMapPoints.push_back(new MapPoint(mMapPoints.size(), position));
    return mMapPoints.back();
}

size_t Map::mapPointsSize() const {
    std::shared_lock lock(mMapMutex);
    return mMapPoints.size();
}

void Map::run(Map* map) {
    while (!map->mIsFinish) {
        // TODO optimization part here
    }
}
