#include "roarm_ik.hh"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>

ik_solver::ik_solver(roarm* arm) : arm(arm) {}

std::optional<std::vector<double>> ik_solver::generate_ik(
    double phi, double x, double y, double z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = phi;

    constexpr double pi = 3.14159265358979323846;
    constexpr double deg_to_rad = pi / 180.0;
    constexpr double rad_to_deg = 180.0 / pi;

    double phi_rad = phi * deg_to_rad;
    double base_angle_rad = std::atan2(y, x);
    double R = std::sqrt(x * x + y * y);

    double D2x = R - r3 * std::cos(phi_rad);
    double D2y = z - r3 * std::sin(phi_rad);

    double d = std::sqrt(D2x * D2x + D2y * D2y);

    if (r1 <= 0 || r2 <= 0 ||
        d == 0 || d > r1 + r2 || d < std::abs(r1 - r2))
    {
        return std::nullopt;
    }

    double a = std::atan2(D2y, D2x);

    double cos_b =
        (r1 * r1 + d * d - r2 * r2) / (2 * r1 * d);

    double b = std::acos(std::clamp(cos_b, -1.0, 1.0));
    double math_theta1 = a + b;

    double cos_elbow =
        (r1 * r1 + r2 * r2 - d * d) / (2 * r1 * r2);

    double inner_elbow =
        std::acos(std::clamp(cos_elbow, -1.0, 1.0));

    double math_theta2 = pi - inner_elbow;

    double robot_shoulder_deg =
        ((pi / 2) - math_theta1) * rad_to_deg;

    double robot_elbow_deg = math_theta2 * rad_to_deg;
    double robot_base_deg = base_angle_rad * rad_to_deg;

    double robot_wrist_deg =
        90 - robot_shoulder_deg - robot_elbow_deg - phi;

    std::vector<double> result{
        robot_base_deg,
        robot_shoulder_deg,
        robot_elbow_deg,
        robot_wrist_deg,
        0,
        10
    };

    std::copy(result.begin(), result.end(), angles);

    return result;
}

void ik_solver::wait_for_arrival(
    const std::vector<double>& target_angles,
    double tolerance,
    double timeout)
{
    if (!arm || target_angles.size() < 4) {
        return;
    }

    constexpr double rad_to_deg =
        180.0 / 3.14159265358979323846;

    constexpr int joint_offset = 5;
    constexpr int joint_count = 4;

    auto start_time = std::chrono::steady_clock::now();

    while (std::chrono::duration<double>(
               std::chrono::steady_clock::now() - start_time
           ).count() < timeout)
    {
        auto pose = arm->pose_get();

        if (pose.size() >= joint_offset + joint_count &&
            pose[0] == 1051)
        {
            double max_diff = 0.0;
            bool valid = true;

            for (int i = 0; i < joint_count; ++i) {
                double current_deg =
                    pose[joint_offset + i] * rad_to_deg;

                if (!std::isfinite(current_deg) ||
                    !std::isfinite(target_angles[i]))
                {
                    valid = false;
                    break;
                }

                max_diff = std::max(
                    max_diff,
                    std::abs(current_deg - target_angles[i])
                );
            }

            if (valid && max_diff <= tolerance) {
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void ik_solver::move_to_xyz(
    double phi, double x, double y, double z,
    bool wait)
{
    if (!arm) {
        return;
    }

    auto target = generate_ik(phi, x, y, z);

    if (target && arm->joints_angle_ctrl(*target, 500, 254)) {
        if (wait) {
            wait_for_arrival(*target);
        }
    }
}

void ik_solver::draw_line(
    double phi,
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    int steps)
{
    if (!arm || steps <= 0) {
        return;
    }

    std::optional<std::vector<double>> last_valid_angles;

    for (int i = 0; i <= steps; ++i) {
        double t = static_cast<double>(i) / steps;

        double cx = x1 + (x2 - x1) * t;
        double cy = y1 + (y2 - y1) * t;
        double cz = z1 + (z2 - z1) * t;

        auto target = generate_ik(phi, cx, cy, cz);

        if (target) {
            if (!arm->joints_angle_ctrl(*target, 800, 254)) {
                return;
            }

            last_valid_angles = target;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (last_valid_angles) {
        wait_for_arrival(*last_valid_angles);
    }
}