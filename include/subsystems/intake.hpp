#pragma once

#include "dlib/dlib.hpp"
#include "api.h"

enum class Alliance {
    Red,
    Blue
};

class Intake {
public:
    pros::Motor intake_motor;
    pros::Motor intake_motor_2;
    pros::Optical optical_sensor;
    Alliance alliance;
    bool do_sort;
    int8_t direction = 1;

    Intake(
        int8_t motor_port,
        int8_t bottom_motor,
        uint8_t optical_port
    );

    void set_alliance(Alliance alliance);

    void move(uint8_t power);

    void move_voltage(ushort voltage);

    void max(void);

    void bottom_max_top_rev(void);

    void bottom_rev(void);

    void reverse(void);

    void stop(void);

    void toggle_color_sort(void);

    void toggle_direction(void);

};