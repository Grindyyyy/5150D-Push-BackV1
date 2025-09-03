#include "subsystems/pneumatics.hpp"

Pneumatics::Pneumatics(
    char left_piston_port,
    char right_piston_port
) : left_piston(left_piston_port), right_piston(right_piston_port)
{
    left_piston.set_value(false);
    right_piston.set_value(false);
};

void Pneumatics::set_state(bool state){
    left_piston.set_value(state);
    right_piston.set_value(state);
}