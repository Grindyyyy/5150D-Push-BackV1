#include "main.h"
#include "robot.hpp"
#include "au/au.hpp"
#include "subsystems/intake.hpp"
#include "subsystems/brain.hpp"

using namespace au;

dlib::ChassisConfig chassis_config {
	{7},
	{-2},
	pros::MotorGearset::blue,
	rpm(400),
	inches(3.25)
};

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

dlib::PidConfig angular_pid_config {
	{
		17,
		0,
		1
	},
	volts(12)
};

dlib::PidConfig precise_angular_pid_config {
	{

	},
	volts(12)
};

dlib::ErrorDerivativeSettler<Degrees> angular_pid_settler {
	degrees(3),
	degrees_per_second(20)
};

dlib::ErrorDerivativeSettler<Degrees> precise_angular_pid_settler {
	degrees(1.5),
	degrees_per_second(10)
};

dlib::Feedforward<Meters> linear_feedforward {
	{

	}
};

dlib::PidConfig linear_feedforward_pid_config {
	{

	},
	volts(12)
};

dlib::Feedforward<Degrees> angular_feedforward {
	{

	}
};

dlib::PidConfig angular_feedforward_pid_config {
	{

	},
	volts(12)
};

dlib::ErrorDerivativeSettler<Meters> linear_feedforward_settler {
	inches(1),
	meters_per_second(.1)
};

dlib::ErrorDerivativeSettler<Degrees> angular_feedforward_settler {
	degrees(3),
	degrees_per_second(20)
};

Intake intake {
	-11,
	-19, // 19 bottom 20 back
	2
};

Robot robor = {
	chassis_config,
	imu_config,
	intake,
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
}

void disabled() {}

void competition_initialize() {}

void autonomous() { // all coords are in meters btw
	robor.move(0.6076,0.324);
	robor.intake.max();
	robor.move(-0.6076,-0.324);
	robor.move(0.3076,0.1524);
	robor.move(-0.3076,-0.1524);
}

void opcontrol() {
	pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

	robor.chassis.left_motors.raw.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	robor.chassis.right_motors.raw.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);

	while (true) {
		
		// arcade implementation
		robor.chassis.arcade(master.get_analog(ANALOG_LEFT_Y),-master.get_analog(ANALOG_RIGHT_X) * .75);

		// color sort
		if(master.get_digital_new_press(DIGITAL_L2)){
			robor.intake.direction = 1;
		}
		if(master.get_digital_new_press(DIGITAL_L1)){
			robor.intake.direction = -1;
		}


		// intake control
		if(master.get_digital(DIGITAL_R2)){
			robor.intake.max();
		}
		else if(master.get_digital(DIGITAL_R1)){
			robor.intake.bottom_rev();
		}
		else if(master.get_digital(DIGITAL_L1)){
			robor.intake.bottom_max_top_rev();
		}
		else if(master.get_digital(DIGITAL_L2)){
			robor.intake.reverse();
		}
		else{
			robor.intake.stop();
		}

		pros::delay(20); // Run for 20 ms then update
	}
}