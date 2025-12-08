#pragma once
#include "subsystems/intake.hpp"
#include "subsystems/pneumatics.hpp"
#include "dlib/hardware/timer.hpp"
#include "robot.hpp"
#include "atomic"

extern std::atomic<bool> is_auton;

void left_wp(Intake& intake, Pneumatics& pneumatics, Robot& robor);
void left_elim(Intake& intake, Pneumatics& pneumatics, Robot& robor);
void left_elim_goal(Intake& intake, Pneumatics& pneumatics, Robot& robor);
void right_elim(Intake& intake, Pneumatics& pneumatics, Robot& robor);
void right_elim_goal(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void right_wp(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void skills(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void skills_low(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void solo(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void all_in_goal(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void test_tare(Intake& intake, Pneumatics& pneumatics, Robot& robor);

void test_mp(dlib::Timer& timer, Robot& robor);

void test_trapezoid(Robot& robor);