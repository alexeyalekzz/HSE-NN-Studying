//
// Created by Alexey Klimov on 04.02.2022.
//

#include "drawer.h"

#include <pangolin/display/display.h>
#include <pangolin/gl/opengl_render_state.h>
#include <pangolin/handler/handler.h>
#include <pangolin/display/view.h>

#include "map.h"
#include "map_point.h"
#include "frame.h"


inline void drawGroundTruth(const std::vector<Eigen::Isometry3d>& gt) {
    glBegin(GL_LINES);
    for (size_t i = 1; i < gt.size(); i++)
    {
        glColor3f(Drawer::GT_COLOR[0], Drawer::GT_COLOR[1], Drawer::GT_COLOR[2]);
        auto t1 = gt[i-1].translation(), t2 = gt[i].translation();
        glVertex3d(t1[0], t1[1], t1[2]);
        glVertex3d(t2[0], t2[1], t2[2]);
    }
    glEnd();
}

inline void drawMapPoints(const std::vector<MapPoint*>& points) {
    if (points.empty()) return;

    glPointSize(1);
    glColor3f(Drawer::MP_COLOR[0], Drawer::MP_COLOR[1], Drawer::MP_COLOR[2]);

    glBegin(GL_POINTS);
    for (const MapPoint* mp : points) {
        glVertex3f(mp->mWorldPos.x(), mp->mWorldPos.y(), mp->mWorldPos.z());
    }
    glEnd();
}

inline void drawKeyFrames(const std::vector<Frame*>& frames) {
    if (frames.empty()) return;

    glBegin(GL_LINES);
    Frame* prev = *frames.begin();
    for (auto& f : frames)
    {
        if (!f->isKeyFrame()) continue;

        // draw three axes of each pose
        const auto& R = f->pose().so3();
        const auto& Ow = f->pose().translation();
        Eigen::Vector3d Xw = R * (0.1 * Eigen::Vector3d(1, 0, 0)) + Ow;
        Eigen::Vector3d Yw = R * (0.1 * Eigen::Vector3d(0, 1, 0)) + Ow;
        Eigen::Vector3d Zw = R * (0.1 * Eigen::Vector3d(0, 0, 1)) + Ow;
        glColor3f(1.0, 0.0, 0.0);
        glVertex3d(Ow[0], Ow[1], Ow[2]);
        glVertex3d(Xw[0], Xw[1], Xw[2]);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3d(Ow[0], Ow[1], Ow[2]);
        glVertex3d(Yw[0], Yw[1], Yw[2]);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3d(Ow[0], Ow[1], Ow[2]);
        glVertex3d(Zw[0], Zw[1], Zw[2]);

        // draw a connection
        if (f == prev) continue;

        glColor3f(Drawer::KF_COLOR[0], Drawer::KF_COLOR[1], Drawer::KF_COLOR[2]);
        const auto& t1 = prev->pose().translation();
        glVertex3d(t1[0], t1[1], t1[2]);
        glVertex3d(Ow[0], Ow[1], Ow[2]);
        prev = f;
    }
    glEnd();

    // mark keyframes
    glPointSize(7);
    glColor3f(Drawer::KF_COLOR[0], Drawer::KF_COLOR[1], Drawer::KF_COLOR[2]);
    glBegin(GL_POINTS);
    for (auto& f : frames) {
        if (f->isKeyFrame()) {
            const auto& Ow = f->pose().translation();
            glVertex3d(Ow[0], Ow[1], Ow[2]);
        }
    }
    glEnd();
}

/////////////////////////////////////////////////////// Drawer ///////////////////////////////////////////////////////

Drawer::Drawer(const Map& map)
    : mMap(map)
    , mThread([this]() {
        pangolin::CreateWindowAndBind("VO", 640, 480);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POINT_SMOOTH);
        glLineWidth(2.);

        pangolin::OpenGlRenderState s_cam(
            pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
            pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
        );

        pangolin::Handler3D handler(s_cam);
        pangolin::View& d_cam = pangolin::CreateDisplay()
                                    .SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f)
                                    .SetHandler(&handler);

        while(!pangolin::ShouldQuit() && !mIsFinish) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            d_cam.Activate(s_cam);

            if (!mMap.mGTPoses.empty())
                drawGroundTruth(mMap.mGTPoses);

//            {
//                std::shared_lock lock(mMap.mMapMutex);
//                drawKeyFrames(mMap.mAllFrames);
//                drawMapPoints(mMap.mMapPoints);
//            }

            glClearColor(1., 1., 1., 1.);
            pangolin::FinishFrame();
        }
    })
{}

Drawer::~Drawer() {
    mIsFinish = true;
    mThread.join();
}
