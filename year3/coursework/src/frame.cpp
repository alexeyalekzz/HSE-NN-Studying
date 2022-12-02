//
// Created by Alexey Klimov on 28.05.2022.
//
#ifdef _WIN32
#define _CRT_USE_BUILTIN_OFFSETOF
#endif

#include "frame.h"


Frame::Frame(size_t frameID, bool is_kf)
    : ID(frameID)
    , mIsKeyFrame(is_kf)
    , mCameraPose(Sophus::SE3d())
{}

StereoFrame::StereoFrame(size_t frameID, bool is_kf, cv::Mat left, cv::Mat right)
    : Frame(frameID, is_kf)
    , mLeftImg(std::move(left))
    , mRightImg(std::move(right))
{}

void StereoFrame::setFeatures(std::vector<Feature::Ptr>&& left, std::vector<Feature::Ptr>&& right) {
    mLeftFeatures = std::move(left);
    mRightFeatures = std::move(right);
}

void StereoFrame::insertFeatures(const std::vector<Feature::Ptr>& left, const std::vector<Feature::Ptr>& right) {
    mLeftFeatures.reserve(mLeftFeatures.size() + left.size());
    mLeftFeatures.insert(mLeftFeatures.end(), left.begin(), left.end());

    mRightFeatures.reserve(mRightFeatures.size() + right.size());
    mRightFeatures.insert(mRightFeatures.end(), right.begin(), right.end());
}

std::vector<cv::Point2f> StereoFrame::leftPoints() const {
    std::vector<cv::Point2f> res;
    res.reserve(mLeftFeatures.size());
    for (auto& f : mLeftFeatures)
        res.push_back(f->pos);
    return res;
}

std::vector<cv::Point2f> StereoFrame::rightPoints() const {
    std::vector<cv::Point2f> res;
    res.reserve(mRightFeatures.size());
    for (auto& f : mRightFeatures)
        res.push_back(f->pos);
    return res;
}
