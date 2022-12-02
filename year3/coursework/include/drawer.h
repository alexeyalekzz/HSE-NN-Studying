//
// Created by Alexey Klimov on 04.02.2022.
//

#pragma once

#include <thread>
#include <atomic>


class Map;


class Drawer {
public:

    static constexpr float KF_COLOR[] = { 0., 0., 1. };     // Yellow
    static constexpr float GT_COLOR[] = { 0., 1., 0. };     // Green
    static constexpr float TRAJ_COLOR[] = { 1., 1., 1. };   // White
    static constexpr float MP_COLOR[] = { 0., 0., 0. };     // Blue

    explicit Drawer(const Map& map);
    ~Drawer();

private:
    std::atomic<bool> mIsFinish = false;
    const Map& mMap;
    std::thread mThread;
};
