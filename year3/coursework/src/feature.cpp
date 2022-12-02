//
// Created by Alexey Klimov on 07.06.2022.
//
#include "feature.h"

Feature::Ptr Feature::Create(const cv::Point2f& p, MapPoint* mp) {
    return std::make_shared<Feature>(p, mp);
}

std::vector<Feature::Ptr> Feature::FromPoints(const std::vector<cv::Point2f>& pts) {
    std::vector<Feature::Ptr> result;
    result.reserve(pts.size());
    for (auto& p : pts)
        result.emplace_back(new Feature(p));
    return result;
}

Feature::Feature(const cv::Point2f& p)
    : pos(p)
    , mapPoint(nullptr)
    , isOutlier(false)
{}

Feature::Feature(const cv::Point2f& p, MapPoint* mp)
    : pos(p)
    , mapPoint(mp)
    , isOutlier(false)
{}
