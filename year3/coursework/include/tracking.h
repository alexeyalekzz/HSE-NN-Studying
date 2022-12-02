//
// Created by Alexey Klimov on 28.05.2022.
//

#pragma once

#include "async_image_loader.h"

#include <opencv2/features2d.hpp>

#include <sophus/se3.hpp>

#include <string>


struct Config;
class Map;
class StereoFrame;

class Tracking {
public:

    explicit Tracking(const Config& config, Map& map, const std::vector<float>& calib_data);
    void startStereo();

private:
    /// Returns frame with images and filled isKeyFrame field or nullptr if it can't load images.
    StereoFrame* nextFrame();

    /// Adds new points to Map, binds them to features
    void triangulateNewMapPoints(StereoFrame* frame);

    /// Find points in current frame from previous
    static void trackFrames(StereoFrame* prev, StereoFrame* curr);

    /// Calculate rotation and translation between frames, remove outliers
    void calculatePose(StereoFrame* frame);

    /// Fill frame with features that observable from BOTH left and right eyes
    void findLeftFeaturesInRight(StereoFrame* frame) const;

    /// Extracts features from left eye.
    void extractFeatures(StereoFrame* frame);

    StereoFrame *prevFrame, *currFrame;
    Sophus::SE3d mRelativeMotion;

    size_t lastFrameID;
    AsyncImageLoader mImageLoader;
    cv::Ptr<cv::FeatureDetector> mDetector;
    const Config& mConfig;
    Map& mMap;

    cv::Matx34f mProjectionMatrixLeft, mProjectionMatrixRight;
    cv::Matx33f K; // Camera intrinsics
    double inlierRatio; // inliers.size() / points.size()
};
