#pragma once

#include "api.h"

class Pneumatics { 
public:
    pros::adi::DigitalOut left_piston;
    pros::adi::DigitalOut right_piston;

    Pneumatics(
        char left_piston_port,
        char right_piston_port
    );

    void set_state(bool state);
    
    void toggle_state(void);
};