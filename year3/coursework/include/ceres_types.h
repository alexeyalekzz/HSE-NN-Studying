//
// Created by Alexey Klimov on 07.06.2022.
//

#pragma once

#include <ceres/ceres.h>
#include <ceres/local_parameterization.h>

#include <sophus/se3.hpp>
#include <sophus/ceres_local_parameterization.hpp>


struct SE3Manifold : ceres::Manifold {
    using SE3 = Sophus::SE3d;
    using JacobianType = Eigen::Matrix<double, SE3::num_parameters, SE3::DoF,
                                       SE3::DoF == 1 ? Eigen::ColMajor : Eigen::RowMajor>;

    inline int AmbientSize() const override { return SE3::num_parameters; }
    inline int TangentSize() const override { return SE3::DoF; }

    inline bool Plus(const double* x, const double* delta_raw, double* x_plus_delta) const override {
        const Eigen::Map<const SE3> T(x);
        auto delta = Sophus::Mapper<SE3::Tangent>::map(delta_raw);
        Eigen::Map<SE3> T_plus_delta(x_plus_delta);
        T_plus_delta = T * SE3::exp(delta);
        return true;
    }

    inline bool Minus(const double* y, const double* x, double* y_minus_x) const override {
        return false;
    }

    inline bool PlusJacobian(const double* x, double* jacobian_raw) const override {
        Eigen::Map<const SE3> T(x);
        Eigen::Map<JacobianType> jacobian(jacobian_raw);
        jacobian = T.Dx_this_mul_exp_x_at_0();
        return true;
    }

    inline bool MinusJacobian(const double* x, double* jacobian_raw) const override {
        return false;
    }
};

struct ReprojectionError {
    inline ReprojectionError(const double observed_x, const double observed_y, const Eigen::Matrix3d& k)
        : obs_x(observed_x), obs_y(observed_y), K(k)
    {}

    template<class T>
    inline bool operator()(const T* const pose,
                           const T* const point,
                           T* residuals) const
    {
        Eigen::Map<const Sophus::SE3<T>> se3(pose);
        Eigen::Map<const Eigen::Vector3<T>> p(point);
        auto res = se3 * p; // transform point from world to camera

        T projected_x = res[0] / res[2] * K(0, 0) + K(0, 2);
        T projected_y = res[1] / res[2] * K(1, 1) + K(1, 2);

        residuals[0] = projected_x - T(obs_x);
        residuals[1] = projected_y - T(obs_y);

        return true;
    }

    static inline auto Create(const double x, const double y, const Eigen::Matrix3d& K) {
        return new ceres::AutoDiffCostFunction<ReprojectionError, 2, 7, 3>(
            new ReprojectionError(x, y, K)
        );
    }

    double obs_x, obs_y;
    const Eigen::Matrix3d& K;
};
