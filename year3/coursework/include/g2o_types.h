//
// Created by Alexey Klimov on 07.06.2022.
//

#pragma once

#include <g2o/core/base_vertex.h>
#include <g2o/core/base_binary_edge.h>

#include <sophus/se3.hpp>

class VertexPose : public g2o::BaseVertex<6, Sophus::SE3d> {
public:
    inline void setToOriginImpl() override { _estimate = Sophus::SE3d{}; }

    inline void oplusImpl(const double *update) override {
        Eigen::Map<const Eigen::Vector<double, 6>> upd(update);
        _estimate = Sophus::SE3d::exp(upd) * _estimate;
    }

    inline bool read(std::istream&) override { return true; }
    inline bool write(std::ostream&) const override { return true; }
};

class VertexXYZ : public g2o::BaseVertex<3, Eigen::Vector3d> {
public:
    inline void setToOriginImpl() override { _estimate.fill(0); }

    inline void oplusImpl(const double *update) override {
        _estimate += Eigen::Map<const Eigen::Vector3d>(update);
    }

    inline bool read(std::istream&) override { return true; }
    inline bool write(std::ostream&) const override { return true; }
};

class EdgeProjection
    : public g2o::BaseBinaryEdge<2, Eigen::Vector2d, VertexPose, VertexXYZ> {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    inline explicit EdgeProjection(const Eigen::Matrix3d& k)
        : K(k)
    {}

    inline void computeError() override {
        const VertexPose *v0 = dynamic_cast<VertexPose*>(_vertices[0]);
        const VertexXYZ *v1 = dynamic_cast<VertexXYZ*>(_vertices[1]);
        Sophus::SE3d T = v0->estimate();
        Eigen::Vector3d pos_pixel = K * (T * v1->estimate());
        pos_pixel /= pos_pixel[2];
        _error = _measurement - pos_pixel.head<2>();
    }

    inline void linearizeOplus() override {
        const VertexPose *v0 = dynamic_cast<VertexPose*>(_vertices[0]);
        const VertexXYZ *v1 = dynamic_cast<VertexXYZ*>(_vertices[1]);
        Sophus::SE3d T = v0->estimate();
        Eigen::Vector3d pw = v1->estimate();
        Eigen::Vector3d pos_cam = T * pw;
        double fx = K(0, 0);
        double fy = K(1, 1);
        double X = pos_cam[0];
        double Y = pos_cam[1];
        double Z = pos_cam[2];
        double Zinv = 1.0 / (Z + 1e-18);
        double Zinv2 = Zinv * Zinv;
        _jacobianOplusXi << -fx * Zinv, 0, fx * X * Zinv2, fx * X * Y * Zinv2,
            -fx - fx * X * X * Zinv2, fx * Y * Zinv, 0, -fy * Zinv,
            fy * Y * Zinv2, fy + fy * Y * Y * Zinv2, -fy * X * Y * Zinv2,
            -fy * X * Zinv;

        _jacobianOplusXj = _jacobianOplusXi.block<2, 3>(0, 0) * T.rotationMatrix();
    }

    inline bool read(std::istream&) override { return true; }
    inline bool write(std::ostream&) const override { return true; }

private:
    const Eigen::Matrix3d& K;
};
