//
// Created by Alexey Klimov on 28.05.2022.
//

#include "config_reader.h"
#include "map.h"
#include "tracking.h"
#include "optimizer.h"

#include <fstream>

// hardcoded for now to parse only 2 and 3 cameras from kitti
std::optional<std::vector<float>> parseCalib(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "-! Failed to load calib.txt file" << std::endl;
        return {};
    }

    std::vector<float> result (24);
    std::string temp;

    std::getline(fin, temp, '\n'); // "P0: ..."
    std::getline(fin, temp, '\n'); // "P1: ..."

    fin >> temp; // "P2:"
    for (int i = 0; i < 12; ++i) {
        fin >> result[i];
    }
    fin >> temp; // "P3:"
    for (int i = 12; i < 24; ++i) {
        fin >> result[i];
    }

    return result;
}


int main() {
    // get values from configuration file
    ConfigReader reader("./configs/config.yaml");
    Config config = reader.getConfig();

    if (auto calib = parseCalib(config.calib_path))
    {
        Map map (config, Optimizer(config, *calib));
        Tracking tracking (config, map, *calib);
        tracking.startStereo();
    }
    else
    {
        return -1;
    }
}
