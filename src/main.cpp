#include "main.h"
#include "dlib/hardware/timer.hpp"
#include "robot.hpp"
#include "au/au.hpp"
#include "subsystems/intake.hpp"
#include "subsystems/brain.hpp"
#include "autos.h"
#include "controls/controls.h"

using namespace au;

dlib::ChassisConfig chassis_config {
	{-12,-13,-1},
	{19,18,17},
	pros::MotorGearset::blue,
	rpm(400),
	inches(3.25)
};

dlib::Timer timer {};

dlib::ImuConfig imu_config {
	16,	// imu port
	1	// optional imu scaling constant
};

dlib::PidConfig linear_pid_config {
	{
		40,
		0,
		0
	},
	volts(12)
};

dlib::ErrorDerivativeSettler<Meters> linear_pid_settler {
	inches(1),
	meters_per_second(.1)
};

/*dlib::PidConfig angular_pid_config {
	{
		45,
		0,
		3.6
	},
	volts(12)
};*/

dlib::PidConfig angular_pid_config {
	{
		42,
		0,
		2.45
	},
	volts(7)
};

dlib::PidConfig precise_angular_pid_config {
	{
		42,
		0,
		2.45
	},
	volts(7)
};

dlib::ErrorDerivativeSettler<Degrees> angular_pid_settler {
	degrees(2),
	degrees_per_second(15)
};

dlib::ErrorDerivativeSettler<Degrees> precise_angular_pid_settler {
	degrees(0.5),
	degrees_per_second(15)
};

dlib::Feedforward<Meters> linear_feedforward {
	{
		1.0988731952658297,
		5.6,
		1.3
	}
};

dlib::PidConfig linear_feedforward_pid_config {
	{50,0,

	},
	volts(12)
};

dlib::Feedforward<Degrees> angular_feedforward { // Angular FFWD Gains
	{
		2.07524188995,
		0.0158736421342,
		0//0.00524090129067
	}
};

dlib::PidConfig angular_feedforward_pid_config {
	{
		30,
		0,
		0
	},
	volts(12)
};

dlib::ErrorDerivativeSettler<Meters> linear_feedforward_settler {
	inches(.5),
	meters_per_second(.1)
};

dlib::ErrorDerivativeSettler<Degrees> angular_feedforward_settler {
	degrees(3),
	degrees_per_second(20)
};


Intake intake {-15,-6,-10};

Pneumatics pneumatics {
	'H',
	'F',
	'E'
};

Robot robor = {
	chassis_config,
	imu_config,
	intake,
	pneumatics,
	linear_pid_config,
	linear_pid_settler,
	angular_pid_config,
	precise_angular_pid_config,
	angular_pid_settler,
	precise_angular_pid_settler,
	linear_feedforward,
	linear_feedforward_pid_config,
	angular_feedforward,
	angular_feedforward_pid_config,
	linear_feedforward_settler,
	angular_feedforward_settler,
};


void initialize() {
	robor.initialize();
	initialize_brain();
	robor.start_odom();

	robor.chassis.left_motors.raw.tare_position_all();
	robor.chassis.right_motors.raw.tare_position_all();

	pros::Task print_coordinates([&](){
		while(true){
			// SCREEN TASK -- Prints coordinates to our GUI
			dlib::Pose2d pose = robor.odom.get_position();
			print_coords(robor);
			// battery update for gui 
			update_battery_percent();	
			pros::delay(20);
		}	
	});
	pros::Task do_intake_control([&](){
		while(true){
			control_intake_task(intake);
			pros::delay(20);
		}
		
	});
	pros::Task do_intake_jam([&](){
		while(true){
			control_jam(intake);
			pros::delay(20);
		}
	});
}

void disabled() {}

void competition_initialize() {}

void run_auton(){
	if(get_selected() == 1){
		solo(intake,pneumatics,robor);
	}
	else if(get_selected() == 2){
		left_elim(intake,pneumatics,robor);
	}
	else if(get_selected() == 3){
		right_elim(intake,pneumatics,robor);
	}
	else if(get_selected() == 4){
		skills(intake,pneumatics,robor);
	}
}

void autonomous() { // all coords are in meters btw
	//right_elim(intake,pneumatics,robor);
	
	//robor.chassis.turn_voltage(volts(1.5));
	//test_mp(timer, robor);

	/*robor.chassis.turn_voltage(volts(2.07524188995+(0.0148736421342*500)));

	double last_position = 0;
	while(true){
		double current_position = robor.imu.get_rotation().in(degrees);
		double velocity = (current_position - last_position) / 0.05;

		std::cout << velocity << std::endl;

		last_position = current_position;

		pros::delay(50);
	}*/
	//run_auton();
	//skills(intake,pneumatics,robor);
	left_elim(intake,pneumatics,robor);
	//right_elim_goal(intake,pneumatics,robor);
	//solo(intake,pneumatics,robor);
	
}



void opcontrol() {
	pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

	robor.chassis.left_motors.raw.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	robor.chassis.right_motors.raw.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	//pneumatics.set_descore(true);
	while (true) {
		
		control_drive(robor, master);
		control_pneumatics(pneumatics, master);
		control_intake(intake, master);

		pros::delay(15); // Run for 20 ms then update
	}
}