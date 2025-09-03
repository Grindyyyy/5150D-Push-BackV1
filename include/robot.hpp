#pragma once
#include "dlib/controllers/error_derivative_settler.hpp"
#include "dlib/dlib.hpp"
#include "subsystems/intake.hpp"
#include "subsystems/pneumatics.hpp"
#include "au/au.hpp"

using namespace au;

class Robot {
public:
	// Drivebase
	dlib::Chassis chassis;
	dlib::Imu imu;

    // Intake
    Intake intake;

    // Pneumatics
    Pneumatics pneumatics;

	// Linear PID Controllers
	dlib::Pid<au::Meters> linear_pid;
	dlib::ErrorDerivativeSettler<au::Meters> linear_pid_settler;

	// Angular PID Controllers
	dlib::Pid<au::Degrees> angular_pid;
    dlib::Pid<au::Degrees> precise_angular_pid;
	dlib::ErrorDerivativeSettler<au::Degrees> angular_pid_settler;
    dlib::ErrorDerivativeSettler<au::Degrees>  precise_angular_pid_settler;

	// Feedforward Controllers
	dlib::Feedforward<au::Meters> linear_feedforward;
    dlib::Pid<au::Meters> linear_feedforward_pid;
	dlib::Feedforward<au::Degrees> angular_feedforward;
    dlib::Pid<au::Degrees> angular_feedforward_pid;
    dlib::ErrorDerivativeSettler<au::Meters> linear_feedforward_settler;
    dlib::ErrorDerivativeSettler<au::Degrees> angular_feedforward_settler;

	// Odometry
	dlib::Odometry odom = dlib::Odometry();
	std::unique_ptr<pros::Task> odometry_updater = nullptr;

	// ------------------------------------ //
	//         Robot Class Methods          //
	// ------------------------------------ //

	void initialize();

    // move controllers
    void move_pid(au::Quantity<au::Meters, double> displacement);
    void move_pid(double inches);

    void move_feedforward(double displacement, double max_velocity);
    
    // turn controllers
    void turn_absolute(au::Quantity<au::Degrees, double> heading);
    void turn_absolute(double degrees);

    void turn_relative(au::Quantity<au::Degrees, double> heading);
    void turn_relative(double degrees);

    void turn_precise(au::Quantity<au::Degrees, double> heading);
    void turn_precise(double degrees);

    // primary movements
    void move(double x, double y, double max_velocity = 1.6, bool reverse = false, bool precise_turn = false);
    void turn(double x, double y, bool reverse = false);
    void turn_with_precision(double x, double y, bool reverse = false);

    // odometry task
    void start_odom();	
};