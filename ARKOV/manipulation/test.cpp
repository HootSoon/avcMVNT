#include "roarm_ik.hh"

int main()
{
    roarm arm("/dev/ttyUSB0", 115200);

    if (!arm.init()) {
        return 1;
    }

    arm.poll_pose();

    ik_solver solver(&arm);

    // phi in degrees; coordinates in inches.
    solver.move_to_xyz(0, 14, 0, 10);
    solver.move_to_xyz(0, 14, 2, 10);

    arm.clean_up();
    return 0;
}