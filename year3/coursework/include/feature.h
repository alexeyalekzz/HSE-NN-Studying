//
// Created by Alexey Klimov on 07.06.2022.
//

#pragma once

#include <opencv2/core/types.hpp>

#include <memory>
#include <vector>


class MapPoint;

struct Feature {
    using Ptr = std::shared_ptr<Feature>;

    static Feature::Ptr Create(const cv::Point2f& p, MapPoint* mp);
    static std::vector<Feature::Ptr> FromPoints(const std::vector<cv::Point2f>& pts);

    explicit Feature(const cv::Point2f& p);
    Feature(const cv::Point2f& p, MapPoint* mp);

    cv::Point2f pos;
    MapPoint* mapPoint;
    bool isOutlier;
};
