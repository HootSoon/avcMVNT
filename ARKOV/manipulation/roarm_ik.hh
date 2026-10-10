#ifndef ROARM_IK_HH
#define ROARM_IK_HH

#include <optional>
#include <vector>

#include "roarm.hh"

class ik_solver {
public:
    explicit ik_solver(roarm* arm = nullptr);

    double r1 = 10;
    double r2 = 7.5;
    double r3 = 7;

    double pos[4] = {0.0, 0.0, 0.0, 0.0};
    double angles[6] = {0.0, 0.0, 90.0, 0.0, 0.0, 10.0};

    std::optional<std::vector<double>> generate_ik(
        double phi, double x, double y, double z);

    void move_to_xyz(
        double phi, double x, double y, double z,
        bool wait = true);

    void draw_line(
        double phi,
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        int steps = 30);

    void wait_for_arrival(
        const std::vector<double>& target_angles,
        double tolerance = 2.0,
        double timeout = 3.0);

private:
    roarm* arm;
};

#endif