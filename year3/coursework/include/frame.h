//
// Created by Alexey Klimov on 28.05.2022.
//
#pragma once

#include "feature.h"

#include <sophus/se3.hpp>

#include <opencv2/core/mat.hpp>


class Frame {
public:
    size_t ID;

    explicit Frame(size_t frameID, bool is_kf);

    [[nodiscard]] inline bool isKeyFrame() const { return mIsKeyFrame; }
    [[nodiscard]] inline Sophus::SE3d& pose() { return mCameraPose; }

private:
    bool mIsKeyFrame;
    Sophus::SE3d mCameraPose;
};


class StereoFrame : public Frame {
public:
    StereoFrame(size_t frameID, bool is_kf, cv::Mat left, cv::Mat right);

    void setFeatures(std::vector<Feature::Ptr>&& left, std::vector<Feature::Ptr>&& right);
    void insertFeatures(const std::vector<Feature::Ptr>& left, const std::vector<Feature::Ptr>& right);

    [[nodiscard]] inline size_t countPts() const { return mLeftFeatures.size(); }

    [[nodiscard]] inline cv::Mat& leftImg() { return mLeftImg; }
    [[nodiscard]] inline cv::Mat& rightImg() { return mRightImg; }

    [[nodiscard]] inline const std::vector<Feature::Ptr>& leftFeatures() const { return mLeftFeatures; }
    [[nodiscard]] inline const std::vector<Feature::Ptr>& rightFeatures() const { return mRightFeatures; }

    [[nodiscard]] std::vector<cv::Point2f> leftPoints() const;
    [[nodiscard]] std::vector<cv::Point2f> rightPoints() const;

private:
    cv::Mat mLeftImg, mRightImg;
    std::vector<Feature::Ptr> mLeftFeatures, mRightFeatures;
};
