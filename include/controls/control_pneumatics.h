#pragma once
#include "api.h"
#include "pros/misc.hpp"
#include "subsystems/pneumatics.hpp"
#include "atomic"

extern std::atomic<bool> nanner;
extern std::atomic<bool> loader;
extern std::atomic<bool> descore;

void control_pneumatics(Pneumatics& pneumatics, pros::Controller& master);