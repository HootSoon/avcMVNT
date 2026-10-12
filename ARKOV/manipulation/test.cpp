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
    // solver.move_to_xyz(0, 14, 0, 10, 50, 10);
    // solver.move_to_xyz(0, 14, 2, 10, 50, 10);
    // solver.move_to_xyz(0, 14, 6, 10, 30, 10);
    // solver.move_to_xyz(0, 14, -6, 10, 30, 10);
    // solver.move_to_xyz(0, 14, 0, 6, 30, 10);
    // solver.move_to_xyz(20, 14, 0, 6, 30, 10);
    // solver.move_to_xyz(60, 14, 0, 6, 30, 10);
    // solver.move_to_xyz(-60, 14, 0, 6, 30, 10);
    // solver.move_to_xyz(0, 14, 0, 10, 50, 10);
    // solver.draw_line(0,12,0,10,20,4,4,50,10);
    solver.move_to_xyz(0, 14, 0, 10, 50, 10);
    solver.move_to_xyz(-65, 14, 0, 2, 50, 10);
    solver.move_to_xyz(-85, 14, 0, 2, 50, 10);
    solver.move_to_xyz(-85, 14, 0, 0, 50, 10);
    solver.move_to_xyz(-85, 14, 0, -2, 10, 10);
    //solver.move_to_xyz(-85, 14, 0, -4, 10, 10);
    //solver.move_to_xyz(-85, 14, 0, -6, 10, 10);
    //solver.move_to_xyz(-85, 14, 0, -8, 10, 10);
    //solver.move_to_xyz(0, 14, 0, 10, 50, 10);


    arm.clean_up();
    return 0;
}