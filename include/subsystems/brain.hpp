#pragma once
#include "liblvgl/lvgl.h"
#include "robot.hpp"


void initialize_brain();
void print_coords(Robot& robot);
void update_battery_percent();
int get_selected();