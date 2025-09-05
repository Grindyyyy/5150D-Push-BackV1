#include "subsystems/pneumatics.hpp"

Pneumatics::Pneumatics(
    char left_piston_port,
    char right_piston_port,
    char nanner_port
) : left_piston(left_piston_port), right_piston(right_piston_port), nanner(nanner_port)
{
    left_piston.set_value(false);
    right_piston.set_value(false);
    nanner.set_value(false);
};

void Pneumatics::set_state(bool state){
    left_piston.set_value(state);
    right_piston.set_value(state);
}

void Pneumatics::set_nanner(bool state){
    nanner.set_value(state);
}