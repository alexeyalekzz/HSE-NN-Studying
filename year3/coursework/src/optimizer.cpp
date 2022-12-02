//
// Created by Alexey Klimov on 12.06.2022.
//

#include "optimizer.h"

#include "frame.h"
#include "map_point.h"


Optimizer::Optimizer(const Config& config, const std::vector<float>& c)
    : mConfig(config)
    , K{{ c[0], c[1], c[2] },
        { c[4], c[5], c[6] },
        { c[8], c[9], c[10] }}
{}

#ifdef USE_CERES

#define _CRT_USE_BUILTIN_OFFSETOF
#include "ceres_types.h"

void Optimizer::localBA(std::map<size_t, StereoFrame*>& kframes) {
    ceres::Problem problem;
    ceres::Manifold* se3manifold = new SE3Manifold;
    std::map<size_t, Sophus::SE3d> poses_to_optimize;

    // pick last SLIDING_WINDOW_SIZE frames
    int j = 0;
    for (auto it = kframes.rbegin(); j != SLIDING_WINDOW_SIZE && it != kframes.rend(); ++it) {
        poses_to_optimize.insert({ it->first, it->second->pose().inverse() });
        j++;
    }

    for (auto& [id, pose] : poses_to_optimize) {
        problem.AddParameterBlock(pose.data(), Sophus::SE3d::num_parameters, se3manifold);

        for (auto& f : kframes[id]->leftFeatures()) {
            if (!f->isOutlier) {
                ceres::CostFunction* cost = ReprojectionError::Create(f->pos.x, f->pos.y, K);
                ceres::LossFunction* loss = nullptr; // Squared loss

                problem.AddResidualBlock(
                    cost, loss,
                    pose.data(),
                    f->mapPoint->mWorldPos.data()
                );
            }
        }
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::LinearSolverType::SPARSE_SCHUR;
    options.minimizer_progress_to_stdout = false;
    options.max_num_iterations = 10;
    options.num_threads = 4;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << std::endl;

    for (auto& [id, pose] : poses_to_optimize) {
        kframes[id]->pose() = pose.inverse();
    }
}

#else

#define _CRT_USE_BUILTIN_OFFSETOF
#include "g2o_types.h"

#include <g2o/core/block_solver.h>
#include <g2o/core/robust_kernel_impl.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>


void Optimizer::localBA(std::map<size_t, StereoFrame*>& kframes) {
    double chi2_th = 5.991;

    typedef g2o::BlockSolver_6_3 BlockSolverType;
    typedef g2o::LinearSolverEigen<BlockSolverType::PoseMatrixType> LinearSolverType;
    auto solver = new g2o::OptimizationAlgorithmLevenberg(
        g2o::make_unique<BlockSolverType>(g2o::make_unique<LinearSolverType>())
    );
    g2o::SparseOptimizer optimizer;
    optimizer.setAlgorithm(solver);

    std::unordered_map<size_t, VertexPose*> poses_to_optimize;
    std::unordered_map<size_t, VertexXYZ*> points_to_optimize;

    size_t maxId = 0;
    for (auto& [id, kf] : kframes) {
        auto v = new VertexPose;
        v->setEstimate(kf->pose().inverse());
        v->setId(static_cast<int>(id));
        optimizer.addVertex(v);
        poses_to_optimize[id] = v;

        if (maxId < id) {
            maxId = id;
        }
    }
    maxId += 1;

    for (auto& [id, kf] : kframes) {
        for (auto& f : kf->leftFeatures()) {
            if (optimizer.vertex(static_cast<int>(maxId + f->mapPoint->ID))) continue;
            if (!f->isOutlier) {
                auto v = new VertexXYZ;
                v->setEstimate(f->mapPoint->mWorldPos);
                v->setId(static_cast<int>(maxId + f->mapPoint->ID));
                v->setMarginalized(true);
                optimizer.addVertex(v);
                points_to_optimize[f->mapPoint->ID] = v;

                auto rk = new g2o::RobustKernelHuber();
                rk->setDelta(chi2_th);

                auto e = new EdgeProjection(K);
                e->setVertex(0, poses_to_optimize[kf->ID]);
                e->setVertex(1, v);
                e->setMeasurement({ f->pos.x, f->pos.y });
                e->setInformation(Eigen::Matrix2d::Identity());
                e->setRobustKernel(rk);
                optimizer.addEdge(e);
            }
        }
    }

    optimizer.initializeOptimization();
    optimizer.optimize(10);

    for (auto& kf : kframes) {
        kf->pose() = poses_to_optimize[kf->ID]->estimate().inverse();
    }
    for (auto& [id, v] : points_to_optimize) {
        mpoints[id]->mWorldPos = v->estimate();
    }
}

#endif
