#pragma once

#include "atomic"
#include "subsystems/intake.hpp"
#include "api.h"

extern std::atomic<bool> jam;

void control_intake(Intake& intake, pros::Controller& master);

void control_jam(Intake& intake);

void control_intake_task(Intake& intake);