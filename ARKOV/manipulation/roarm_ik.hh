#ifndef ROARM_IK_HH
#define ROARM_IK_HH

#include <vector>

class ik_solver{
public:
    ik_solver();
    
    bool move_to_xyz();
    bool draw_line();

private:
    void wait_for_arrival();
    std::vector<double> generate_ik(int phi, double x, double y, double z);

};

#endif