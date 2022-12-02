//
// Created by Alexey on 17.01.2022.
//
#include <opencv2/core/mat.hpp>

#include "bucket.h"


int FeatureSet::size() const {
    return points.size();
}

void FeatureSet::clear() {
    points.clear();
    ages.clear();
}

void FeatureSet::appendNewFeatures(const std::vector<cv::Point2f>& features) {
    points.insert(points.end(), features.begin(), features.end());
    std::vector<int> ages_new(features.size(), 0);
    ages.insert(ages.end(), ages_new.begin(), ages_new.end());
}

void FeatureSet::bucketingFeatures(const cv::Mat& image, int bucket_size, int features_per_bucket) {
    // This function buckets features
    // image: only use for getting dimension of the image
    // bucket_size: bucket size in pixel is bucket_size*bucket_size
    // features_per_bucket: number of selected features per bucket
    int image_height = image.rows;
    int image_width = image.cols;
    int buckets_nums_height = image_height / bucket_size;
    int buckets_nums_width = image_width / bucket_size;
    int buckets_number = buckets_nums_height * buckets_nums_width;

    std::vector<Bucket> Buckets;
    Buckets.reserve((buckets_nums_height + 1) * (buckets_nums_width + 1));
    // initialize all the buckets
    for (int buckets_idx_height = 0; buckets_idx_height <= buckets_nums_height; buckets_idx_height++)
    {
        for (int buckets_idx_width = 0; buckets_idx_width <= buckets_nums_width; buckets_idx_width++)
        {
            Buckets.emplace_back(features_per_bucket);
        }
    }

    // bucket all current features into buckets by their location
    int buckets_nums_height_idx, buckets_nums_width_idx, buckets_idx;
    for (int i = 0; i < points.size(); ++i)
    {
        buckets_nums_height_idx = points[i].y / bucket_size;
        buckets_nums_width_idx = points[i].x / bucket_size;
        buckets_idx = buckets_nums_height_idx * buckets_nums_width + buckets_nums_width_idx;
        Buckets[buckets_idx].add_feature(points[i], ages[i]);
    }

    // get features back from buckets
    clear();
    for (int buckets_idx_height = 0; buckets_idx_height <= buckets_nums_height; buckets_idx_height++)
    {
        for (int buckets_idx_width = 0; buckets_idx_width <= buckets_nums_width; buckets_idx_width++)
        {
            buckets_idx = buckets_idx_height * buckets_nums_width + buckets_idx_width;
            Buckets[buckets_idx].get_features(*this);
        }
    }

//    std::cout << "current features number after bucketing: " << size() << std::endl;
}

////////////////////////

void Bucket::add_feature(cv::Point2f point, int age) {
    // won't add feature with age > 10;
//    int age_threshold = 10;
//    if (age < age_threshold)
//    {
        // insert any feature before bucket is full
        if (size() < max_size)
        {
            features.points.push_back(point);
            features.ages.push_back(age);
        }
        else
        // insert feature with old age and remove youngest one
        {
            int age_min = features.ages[0];
            int age_min_idx = 0;

            for (int i = 0; i < size(); i++)
            {
                if (age < age_min)
                {
                    age_min = age;
                    age_min_idx = i;
                }
            }
            features.points[age_min_idx] = point;
            features.ages[age_min_idx] = age;
        }
//    }
}

void Bucket::get_features(FeatureSet& current_features) {
    current_features.points.insert(current_features.points.end(), features.points.begin(), features.points.end());
    current_features.ages.insert(current_features.ages.end(), features.ages.begin(), features.ages.end());
}

int Bucket::size()
{
    return features.points.size();
}
