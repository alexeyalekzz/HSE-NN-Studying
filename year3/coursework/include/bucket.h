#ifndef BUCKET_H
#define BUCKET_H

#include <opencv2/core/types.hpp>

#include <vector>


struct FeaturePoint {
    FeaturePoint(size_t id, int age, const cv::Point2f point)
        : ID(id)
        , mAge(age)
        , mPosOnFrame(point)
    {}

    size_t ID;
    int mAge;
    cv::Point2f mPosOnFrame;
};

struct FeatureSet {
    [[nodiscard]] int size() const;
    void clear();

    void appendNewFeatures(const std::vector<cv::Point2f>& features);
    void bucketingFeatures(const cv::Mat& image, int bucket_size, int features_per_bucket);

    friend class Bucket;

    std::vector<cv::Point2f> points;
    std::vector<int> ages;
};


class Bucket
{
public:
    explicit Bucket(int size) : max_size(size){};
    ~Bucket() = default;

    void add_feature(cv::Point2f point, int age);
    void get_features(FeatureSet& current_features);
    int size();

private:
    int id;
    int max_size;

    FeatureSet features;
};

#endif