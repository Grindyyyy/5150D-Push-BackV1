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
    void turn_feedforward(double degrees, double max_velocity);
    
    // turn controllers
    void turn_absolute(au::Quantity<au::Degrees, double> heading, bool precise, double max_time = 1000);
    void turn_absolute(double degrees, double max_time = 2000);

    void turn_relative(au::Quantity<au::Degrees, double> heading, double max_time = 1000);
    void turn_relative(double degrees, double max_time = 2000);

    void turn_precise(au::Quantity<au::Degrees, double> heading, double max_time = 1000);
    void turn_precise(double degrees, double max_time = 2000);

    // primary movements
    void move_boomerang(double x, double y, double max_move,  double kp_turn=3.75, bool reverse=false, double theta=0, double kp_move=175);
    void move(double x, double y, double max_velocity = 1.6, bool reverse = false, bool precise_turn = false);
    void turn(double x, double y, bool reverse = false,double max_time = 1000);
    void turn_with_precision(double x, double y, bool reverse = false, double max_time = 1000);

    // odometry task
    void start_odom();	
};