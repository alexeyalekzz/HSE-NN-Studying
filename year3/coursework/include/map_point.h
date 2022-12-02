
#pragma once

#include "feature.h"

#include <Eigen/Core>

#include <vector>


class MapPoint {
    friend class Map;

public:
    const size_t ID;

    inline void addObservation(size_t frame_id, const Feature::Ptr& obs) {
        mObservations.insert({frame_id, obs});
    }

private:

    inline MapPoint(size_t id, const Eigen::Vector3d& worldPos)
        : ID(id)
        , mWorldPos(worldPos)
    {}

public:

    Eigen::Vector3d mWorldPos;
    std::unordered_map<size_t, Feature::Ptr> mObservations;
};
