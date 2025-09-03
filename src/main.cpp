#include "main.h"
#include "robot.hpp"
#include "au/au.hpp"
#include "subsystems/intake.hpp"
#include "subsystems/brain.hpp"

using namespace au;

dlib::ChassisConfig chassis_config {
	{-12,-13,-14},
	{19,18,17},
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
	-15,
	-6, // 19 bottom 20 back
	-10
};

Pneumatics pneumatics {
	'A',
	'B'
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
}

void disabled() {}

void competition_initialize() {}

// autons start here:
void left_elim(void){

}

void right_elim(void){

}

void left_wp(void){

}

void right_wp(void){

}

void skills(void){

}

void run_auton(){
	if(get_selected() == 1){
		left_elim();
	}
	else if(get_selected() == 2){
		right_elim();
	}
	else if(get_selected() == 3){
		left_elim();
	}
	else if(get_selected() == 4){
		right_elim();
	}
	else if(get_selected() == 5){
		left_wp();
	}
	else if(get_selected() == 6){
		right_wp();
	}
	else if(get_selected() == 7){
		left_wp();
	}
	else if(get_selected() == 8){
		right_wp();
	}
	else if(get_selected() == 9){
		skills();
	}
}

void autonomous() { // all coords are in meters btw
	run_auton(); // runs the auton that is selected on the gui
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