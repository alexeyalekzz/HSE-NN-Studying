//
// Created by Alexey Klimov on 28.05.2022.
//
#ifdef _WIN32
#define _CRT_USE_BUILTIN_OFFSETOF
#endif

#include "tracking.h"

#include "config_reader.h"
#include "frame.h"
#include "map.h"
#include "map_point.h"
#include "utils.h"

#include <opencv2/calib3d.hpp>
#include <opencv2/video/tracking.hpp>

#include <unordered_set>


Tracking::Tracking(const Config& config, Map& map, const std::vector<float>& c)
    : mImageLoader(config.path, config.start_frame, config.end_frame, true)
    , mMap(map)
    , lastFrameID(0)
    , mConfig(config)
    , K(c[0], c[1], c[2], c[4], c[5], c[6], c[8], c[9], c[10])
    , prevFrame(nullptr)
    , currFrame(nullptr)
    , inlierRatio(0)
    , mProjectionMatrixLeft(c.data())
    , mProjectionMatrixRight(c.data() + 12)
{
    // Initialize detector
    if (config.use_orb) {
        // Parameters that are not set via the configuration file
        int edge_threshold = config.orb_params.patch_size;
        int first_level = 0;
        int WTA_K = 4;
        cv::ORB::ScoreType score_type = cv::ORB::HARRIS_SCORE;

        mDetector = cv::ORB::create(
            config.orb_params.nfeatures,
            config.orb_params.scale_factor,
            config.orb_params.pyr_levels,
            edge_threshold,
            first_level,
            WTA_K,
            score_type,
            config.orb_params.patch_size,
            config.orb_params.fast_treshold
        );
    } else {
        mDetector = cv::FastFeatureDetector::create(
            config.fast_params.threshold,
            config.fast_params.nonMaxSuppression
        );
    }
}

StereoFrame* Tracking::nextFrame() {
    cv::Mat imageLeft, imageRight;
    if (!mImageLoader.get(imageLeft, imageRight)) {
        if (lastFrameID != mConfig.end_frame)
            std::printf("-! Error reading frame #%lld\n", lastFrameID);
        return nullptr;
    }

    bool isKeyFrame = lastFrameID == 0 ||
                      !prevFrame->isKeyFrame() && (
                          prevFrame->countPts() < mConfig.tracking.features_to_track ||
                          inlierRatio < 0.5
                      );
    return new StereoFrame(lastFrameID++, isKeyFrame, std::move(imageLeft), std::move(imageRight));
}

void Tracking::extractFeatures(StereoFrame* frame) {
    // mask to not detect same features again
    cv::Mat mask(frame->leftImg().size(), CV_8UC1, 255);
    for (const auto& f : prevFrame->leftFeatures()) {
        cv::rectangle(mask, f->pos - cv::Point2f(10, 10), f->pos + cv::Point2f(10, 10), 0, cv::FILLED);
    }

    std::vector<cv::KeyPoint> keypoints;
    mDetector->detect(frame->leftImg(), keypoints, mask);

    std::vector<cv::Point2f> newPoints;
    newPoints.reserve(keypoints.size());
    cv::KeyPoint::convert(keypoints, newPoints);

    // todo feature bucketing ???

    frame->setFeatures(Feature::FromPoints(newPoints), {});
    findLeftFeaturesInRight(frame);
}

void Tracking::findLeftFeaturesInRight(StereoFrame* frame) const {
    std::vector<Feature::Ptr> newLeftFeatures, newRightFeatures;
    std::vector<cv::Point2f> rightPoints;

    cv::TermCriteria tc (cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 0.001);
    std::vector<uchar> status;
    cv::Mat error;
    cv::calcOpticalFlowPyrLK(
        frame->leftImg(), frame->rightImg(),
        frame->leftPoints(), rightPoints,
        status, error, cv::Size(11, 11), 3, tc
    );

    newLeftFeatures.reserve(status.size());
    newRightFeatures.reserve(status.size());
    for (size_t i = 0; i < status.size(); ++i) {
        if (status[i] && (std::abs(rightPoints[i].y - frame->leftFeatures()[i]->pos.y) < mConfig.tracking.y_threshold)) {
            newLeftFeatures.push_back(frame->leftFeatures()[i]);
            newRightFeatures.push_back(Feature::Create(rightPoints[i], frame->leftFeatures()[i]->mapPoint));
        }
    }

    // We will keep in left only features that was found in right
    frame->setFeatures(std::move(newLeftFeatures), std::move(newRightFeatures));
}

void Tracking::triangulateNewMapPoints(StereoFrame* frame) {
    cv::Mat pointsHomogeneous;
    std::vector<cv::Point3f> pointsWorld;
    pointsWorld.reserve(frame->countPts());

    cv::triangulatePoints(
        mProjectionMatrixLeft, mProjectionMatrixRight,
        frame->leftPoints(), frame->rightPoints(),
        pointsHomogeneous
    );
    cv::convertPointsFromHomogeneous(pointsHomogeneous.t(), pointsWorld);

    std::vector<Feature::Ptr> newLeftFeatures;
    newLeftFeatures.reserve(frame->countPts());
    for (size_t i = 0; i < pointsWorld.size(); ++i) {
        const auto& p_w = pointsWorld[i];
        if (p_w.z > 0) {
            Eigen::Vector3d p = frame->pose() * Eigen::Vector3d{ p_w.x, p_w.y, p_w.z }; // Transform point

            const auto& leftFeature = frame->leftFeatures()[i];

            auto mp = mMap.createMapPoint(p);
            mp->addObservation(frame->ID, leftFeature); // Add observation from left img
            // todo maybe make stereo observation { x, y, disparity } ???

            leftFeature->mapPoint = mp;
            newLeftFeatures.push_back(leftFeature);
        }
    }

    // Remove right features, we don't need them after triangulation
    frame->setFeatures(std::move(newLeftFeatures), {});
}

void Tracking::trackFrames(StereoFrame* prev, StereoFrame* curr) {
    std::vector<cv::Point2f> currPoints;

    cv::TermCriteria tc (cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 50, 0.001);
    std::vector<uchar> status;
    cv::Mat error;
    cv::calcOpticalFlowPyrLK(
        prev->leftImg(), curr->leftImg(),
        prev->leftPoints(), currPoints,
        status, error, cv::Size(21, 21), 3, tc,
        cv::OPTFLOW_LK_GET_MIN_EIGENVALS
    );

    std::vector<Feature::Ptr> newCurrFeatures;
    newCurrFeatures.reserve(status.size());
    for (size_t i = 0; i < status.size(); ++i) {
        if (status[i]) {
            auto feat = Feature::Create(currPoints[i], prev->leftFeatures()[i]->mapPoint);
            feat->mapPoint->addObservation(curr->ID, feat);
            newCurrFeatures.push_back(feat);
        }
    }

    curr->setFeatures(std::move(newCurrFeatures), {});
    displayPoints(prev->leftImg(), prev->leftPoints());
}

void Tracking::calculatePose(StereoFrame* frame) {
    std::vector<cv::Point3d> worldPoints;
    worldPoints.reserve(frame->countPts());
    for (const auto& feature : frame->leftFeatures()) {
        const auto& pos = feature->mapPoint->mWorldPos;
        worldPoints.emplace_back(pos[0], pos[1], pos[2]);
    }

    cv::Vec3d rvec, tvec;
    std::vector<int> inliersIdxs;
    cv::solvePnPRansac(
        worldPoints,
        frame->leftPoints(),
        K, cv::Mat::zeros(1, 4, CV_64FC1),
        rvec, tvec, false, 100, 8.0, 0.999, inliersIdxs, cv::SOLVEPNP_SQPNP
    );
    inlierRatio = static_cast<double>(inliersIdxs.size()) / frame->countPts();
    cv::Matx33d rotmat;
    cv::Rodrigues(rvec, rotmat);
    cv::Matx44d pose = cv::Matx44d(
        rotmat(0, 0), rotmat(0, 1), rotmat(0, 2), tvec(0),
        rotmat(1, 0), rotmat(1, 1), rotmat(1, 2), tvec(1),
        rotmat(2, 0), rotmat(2, 1), rotmat(2, 2), tvec(2),
                   0,            0,            0,       1
    ).inv();

    Eigen::Vector3d translation (pose(0, 3), pose(1, 3), pose(2, 3));
    Eigen::Matrix3d rotation {
        { pose(0, 0), pose(0, 1), pose(0, 2) },
        { pose(1, 0), pose(1, 1), pose(1, 2) },
        { pose(2, 0), pose(2, 1), pose(2, 2) }
    };

    auto newPose = Sophus::SE3d(rotation, translation);
    auto newRel = newPose * prevFrame->pose().inverse();

    double delta[] = { std::abs(newPose.translation()[0] - prevFrame->pose().translation()[0]),
                       std::abs(newPose.translation()[1] - prevFrame->pose().translation()[1]),
                       std::abs(newPose.translation()[2] - prevFrame->pose().translation()[2]) };

    if (std::any_of(std::begin(delta), std::end(delta), [](auto e) { return e > 10.; })) { // TODO need to adjust
        frame->pose() = mRelativeMotion * prevFrame->pose();
    } else {
        frame->pose() = newPose;
        mRelativeMotion = newRel;
    }

    // Remove outliers
    if (inliersIdxs.size() != frame->countPts()) {
        std::vector<Feature::Ptr> newLeftFeatures; newLeftFeatures.reserve(inliersIdxs.size());
        std::unordered_set s(inliersIdxs.begin(), inliersIdxs.end());
        for (int i = 0; i < frame->countPts(); ++i) {
            if (s.find(i) != s.end()) {
                newLeftFeatures.push_back(frame->leftFeatures()[i]);
            } else {
                frame->leftFeatures()[i]->isOutlier = true;
            }
        }
        frame->setFeatures(std::move(newLeftFeatures), {});
    }
}

void Tracking::startStereo() {
    if (!(prevFrame = nextFrame())) return;
    extractFeatures(prevFrame);
    triangulateNewMapPoints(prevFrame);

    double allTime = 0;

    // Main loop
    while ((currFrame = nextFrame()))
    {
        auto frameStart = Timer::set();

        trackFrames(prevFrame, currFrame);
        calculatePose(currFrame);

        if (currFrame->isKeyFrame()) {
            // use a temporary frame to avoid re-triangulating points that are already on the map
            StereoFrame temp (currFrame->ID, true, currFrame->leftImg(), currFrame->rightImg());
            temp.pose() = currFrame->pose();

            extractFeatures(&temp);
            triangulateNewMapPoints(&temp);

            currFrame->insertFeatures(temp.leftFeatures(), temp.rightFeatures()); // append new features with old
        }

        mMap.addFrame(prevFrame);

        auto frameTime = Timer::get(frameStart).count();
        allTime += frameTime;
        std::printf(
            "%4lld | MPs: %5lld | Time: %.2lfms | Features: %lld | IR: %.2lf%% |%s\n",
            prevFrame->ID, mMap.mapPointsSize(), frameTime,
            prevFrame->countPts(), inlierRatio * 100,
            prevFrame->isKeyFrame() ? " KF" : ""
        );

        prevFrame = currFrame;
    }

    while (1) {}

    std::printf(
        "All time: %lf\n"
        "Avg. frame time: %lf\n",
        allTime, allTime / mConfig.end_frame
    );
}
