//
// Created by Smirnov Grigory on 22.01.2022.
//

#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <opencv2/core.hpp>


struct Config {
    std::string path;
    std::string gt_path;
    std::string calib_path;
    double fx;
    double fy;
    double cx;
    double cy;
    double bf;
    int start_frame;
    int end_frame;
    bool show_gt;
    bool use_orb;

    struct {
        int nfeatures = 500;
        float scale_factor = 1.2;
        int pyr_levels = 8;
        int patch_size = 31;
        int fast_treshold = 20;
    } orb_params;

    struct {
        int threshold = 20;
        bool nonMaxSuppression = true;
    } fast_params;

    struct {
        float y_threshold = 40;
        int features_to_track = 70;
    } tracking;
};

class ConfigReader {
private:
    Config config;
public:
    explicit ConfigReader(const std::string& filename) {
        try {
            cv::FileStorage fs(filename, cv::FileStorage::READ);
            if (fs.isOpened()) {
                fs["path"] >> config.path;
                fs["fx"] >> config.fx;
                fs["fy"] >> config.fy;
                fs["cx"] >> config.cx;
                fs["cy"] >> config.cy;
                fs["bf"] >> config.bf;
                fs["start_frame"] >> config.start_frame;
                fs["end_frame"] >> config.end_frame;
                fs["show_gt"] >> config.show_gt;
                fs["gt_path"] >> config.gt_path;
                fs["use_orb"] >> config.use_orb;
                fs["calib_path"] >> config.calib_path;

                cv::FileNode tracking = fs["tracking_params"];
                tracking["y_threshold"] >> config.tracking.y_threshold;
                tracking["features_to_track"] >> config.tracking.features_to_track;

                cv::FileNode orb_params = fs["orb_params"];
                orb_params["nfeatures"] >> config.orb_params.nfeatures;
                orb_params["scale_factor"] >> config.orb_params.scale_factor;
                orb_params["pyr_levels"] >> config.orb_params.pyr_levels;
                orb_params["patch_size"] >> config.orb_params.patch_size;
                orb_params["fast_treshold"] >> config.orb_params.fast_treshold;

               cv::FileNode fast_params = fs["fast_params"];
               fast_params["threshold"] >> config.fast_params.threshold;
               fast_params["nonMaxSuppression"] >> config.fast_params.nonMaxSuppression;
            }
            else
            {
                std::cout << "Unable to open config file: " << filename << std::endl;
            }
        }
        catch (std::exception& e) {
            std::cout << "Wrong config file!: " << e.what() << std::endl;
        }
    }
    [[nodiscard]] const Config& getConfig() const
    {
        return config;
    }
};
