#pragma once

#include "dlib/dlib.hpp"
#include "api.h"
#include "atomic"

enum class Alliance {
    Red,
    Blue
};

extern std::atomic<int> spin_bottom;
extern std::atomic<int> spin_middle;
extern std::atomic<int> spin_top;
extern std::atomic<bool> anti_jam;

// 0: no 1: max 2: reverse 3: middle

class Intake {
public:
    pros::Motor intake_motor;
    pros::Motor intake_motor_2;
    pros::Motor middle_motor;
    Alliance alliance;
    bool do_sort;
    int8_t direction = 1;
    
    Intake(
        int8_t motor_port,
        int8_t bottom_motor,
        int8_t middle_motor_p
    );
    
    

    void set_alliance(Alliance alliance);

    void move(uint8_t power);

    void move_voltage(ushort voltage);

    void max(void);

    void mid_intake(void);

    void bottom_max(void);

    void bottom_max_driver(void);

    void bottom_max_top_rev(void);

    void bottom_max_back_rev(void);

    void bottom_rev(void);

    void reverse(void);

    void stop(void);

    void toggle_color_sort(void);

    void toggle_direction(void);
    
};