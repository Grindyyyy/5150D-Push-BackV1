#pragma once

#include "api.h"

class Pneumatics { 
public:
    pros::adi::DigitalOut loader;
    pros::adi::DigitalOut nanner;
    pros::adi::DigitalOut descore;

    Pneumatics(
        char loader_port,
        char nanner_port,
        char descore_port
    );

    void set_loader(bool state);

    void set_nanner(bool state);

    void set_descore(bool state);
    
    void toggle_state(void);
};