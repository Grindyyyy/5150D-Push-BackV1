#pragma once

#include "api.h"

class Pneumatics { 
public:
    pros::adi::DigitalOut left_piston;
    pros::adi::DigitalOut right_piston;
    pros::adi::DigitalOut nanner;

    Pneumatics(
        char left_piston_port,
        char right_piston_port,
        char nanner_port
    );

    void set_state(bool state);

    void set_nanner(bool state);
    
    void toggle_state(void);
};