#include "main.h"
#include "dlib/hardware/timer.hpp"
#include "robot.hpp"
#include "au/au.hpp"
#include "subsystems/intake.hpp"
#include "subsystems/brain.hpp"

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

dlib::PidConfig angular_pid_config {
	{
		40,
		0,
		3.325
	},
	volts(12)
};

dlib::PidConfig precise_angular_pid_config {
	{
		40,
		0,
		2.5
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
		1.0457450486840016,
		5.6,
		1.25
	}
};

dlib::PidConfig linear_feedforward_pid_config {
	{25,0,0

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

			//std::cout << robor.odom.get_position().x << "," << robor.odom.get_position().y << "," << robor.odom.get_position().theta << "," << std::endl;

			/*if(intake.intake_motor_2.get_torque() > 0.3 && intake.intake_motor_2.get_actual_velocity() < 2.0){
				intake.intake_motor_2.move(-123);
				pros::delay(100);
				intake.intake_motor_2.move(123);
			}*/

			pros::delay(20);
		}
});
}

void disabled() {}

void competition_initialize() {}

// autons start here:
void left_elim(void){
	intake.bottom_max();
	robor.move(.868,-.043,.9);
	pros::delay(300);
	robor.move(.713,-.026,1.6,true);
	robor.move(.705,.221,.4,false,true);
	intake.mid_intake();
	pros::delay(300);
	robor.chassis.move_voltage(volts(-1.5));
	pros::delay(550);
	robor.chassis.brake();
	robor.move(.771,-.756,1.6,true);
	intake.stop();
	pneumatics.set_loader(true);
	robor.turn_with_precision(-.801,-2.201);
	intake.bottom_max();
	robor.move(.534,-.986,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(600);
	robor.chassis.move_voltage(volts(-2));
	pros::delay(400);
	robor.move(.671,-.857,1.6,true);
	pneumatics.set_loader(false);
	robor.move(.923,-.65,.55,false);
	intake.max();
	pros::delay(800);
	robor.chassis.move_voltage(volts(.75));
}

void right_elim(void){
	intake.bottom_max();
	robor.move(.868,.043,.9);
	pros::delay(300);
	robor.move(.713,.026,1.6,true);
	robor.move(.705,-.221,.4,false,true);
	intake.reverse();
	pros::delay(300);
	robor.chassis.move_voltage(volts(-1.5));
	pros::delay(550);
	robor.chassis.brake();
	robor.move(.771,.756,1.6,true);
	intake.stop();
	pneumatics.set_loader(true);
	robor.turn_with_precision(-.801,2.201);
	intake.bottom_max();
	robor.move(.534,.986,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(600);
	robor.chassis.move_voltage(volts(-2));
	pros::delay(400);
	robor.move(.671,.857,1.6,true);
	pneumatics.set_loader(false);
	robor.move(.923,.65,.55,false);
	intake.max();
	pros::delay(800);
	robor.chassis.move_voltage(volts(.75));
}

void left_wp(void){
	robor.move(.67, .270,1.8);
	robor.turn(.66, -.071);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(.665,-.069,.5); // I sure do hope that it aligns! :)
	robor.chassis.move_voltage(volts(2));
	pros::delay(1000);
	robor.chassis.move_voltage(volts(-3));
	pros::delay(400);
	robor.chassis.brake();
	pneumatics.set_loader(false);
	robor.move(.7,.445,1.15,false,true);
	intake.max();
	pros::delay(1250);
	intake.stop();
	robor.move(.631,.055,1.8,true);
	intake.bottom_max();
	robor.move(-.16,.816,1.15);
	robor.turn_with_precision(-.182,.985);
	intake.mid_intake();
	pros::delay(300);
	robor.chassis.move_voltage(volts(-1.5));
	pros::delay(500);
	robor.chassis.move_voltage(volts(-3));
	pros::delay(400);
	intake.stop();
	robor.chassis.brake();
	robor.move(-.182,.985,1.2,true);
	robor.move(.631,.055,1.8);
	/*robor.move(-.618,.495,1.8);
	intake.bottom_max();
	robor.move(-.939,.77,1.3);
	robor.move(-.698,.980,.7);
	intake.reverse();*/
}

void old_left_wp_2(void){
	intake.bottom_max();
	robor.move(.868,-.043,.9);
	pros::delay(300);
	robor.move(.713,-.026,1.6,true);
	robor.move(.685,.266,.4,false,true);
	intake.mid_intake();
	pros::delay(300);
	robor.chassis.move_voltage(volts(-1.5));
	pros::delay(550);
	robor.chassis.brake();
	robor.move(.771,-.756,1.6,true);
	intake.stop();
	pneumatics.set_loader(true);
	robor.turn_with_precision(-.801,-2.201);
	intake.bottom_max();
	robor.move(.534,-.986,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(600);
	robor.chassis.move_voltage(volts(-2));
	pros::delay(400);
	robor.move(.671,-.857,1.6,true);
	pneumatics.set_loader(false);
	robor.move(.94,-.68,.55,false);
	intake.max();
	pros::delay(1000);
	robor.move(.651,-.878,1.6,true);
	intake.stop();
	robor.move(.187,.016,1);
	intake.bottom_max();
	robor.move(.163,.722,.8);
	pros::delay(300);
	robor.move(.499,.692,.55);
	intake.reverse();
}

void old_left_wp(void){
	intake.bottom_max();
	robor.move(.74,0,.8);
	pros::delay(300);
	robor.move(.73,.2,.33);
	intake.mid_intake();
	pros::delay(300);
	robor.chassis.move_voltage(volts(-2));
	pros::delay(550);
	robor.chassis.brake();
	pros::delay(300);
	intake.stop();
	robor.move(.8,-.7,1.6,true);
	robor.turn(.48,-1.02);
	pneumatics.set_loader(true);
	intake.bottom_max();
	pros::delay(200);
	robor.move(.48,-1.02,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(600);
	robor.chassis.move_voltage(volts(-1.5));
	pros::delay(200);
	robor.move(.63,-.89,1.6,true);
	pneumatics.set_loader(false);
	robor.move(.87,-.64,.6);
	intake.max();
	pros::delay(1000);
	robor.move(.74,-.77,1.6,true);
	intake.stop();
	robor.move(-.05,.24);
	intake.bottom_max();
	robor.move(-.02,.73,.8);
	//robor.move(.2,.72,.4);
	//intake.reverse();
	//.72 .25
}

void test_tare(){
	robor.move(-.5,0);
	robor.imu.tare_position();
	robor.odom.set_position({meters(0),meters(0),degrees(0)});
	robor.move(.5,0);
}

void right_wp(void){
	robor.move(.658, -.270,1.8);
	robor.turn_with_precision(.665, .071);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(.665,.1,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(600);
	robor.chassis.left_motors.move_voltage(volts(-2));
	robor.chassis.right_motors.move_voltage(volts(-3));
	pros::delay(450);
	robor.chassis.brake();
	pneumatics.set_loader(false);
	robor.move(.705,-.46,1.15,false,true);
	intake.max();
	pros::delay(1000);
	intake.stop();
	/*robor.move(.629,-.16,1.8,true);
	robor.move(-.324,-.3,1.8);
	intake.bottom_max();
	robor.move(.162,-.718,1.3);
	robor.move(-.145,-1.02,.7,false,true);
	intake.mid_intake();
	pros::delay(300);
	robor.chassis.move_voltage(volts(-1.5));
	pros::delay(500);
	intake.stop();
	robor.chassis.brake();
	robor.move(-.618,-.495,1.8);
	intake.bottom_max();
	robor.move(-.939,-.77,1.3);
	robor.move(-.698,-.980,.7);
	intake.reverse();*/
}

void skills(void){
	robor.move(.658, .270,1.8);
	robor.turn(.655, -.071);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(.655,-.071,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(2000);
	robor.chassis.move_voltage(volts(-3));
	pros::delay(400);
	robor.chassis.brake();
	pneumatics.set_loader(false);
	robor.move(.685,.467,1.15,false,true);
	intake.max();
	pros::delay(2000);
	robor.chassis.move_voltage(volts(-3));
	pros::delay(500);
	robor.chassis.brake();
	intake.stop();
	intake.bottom_max();
	robor.move(.114,.85,1);
	robor.move(-.060,.94,.55,false,true);
	intake.mid_intake();
	pros::delay(800);
	robor.chassis.move_voltage(volts(-3));
	pros::delay(500);
	robor.chassis.brake();
	intake.stop();
	intake.bottom_max();
	robor.move(.1,1.818,1);
	robor.move(-.078,1.537,.55,false,true);
	intake.reverse();
	pros::delay(2500);
	intake.stop();
	robor.chassis.move_voltage(volts(-3));
	pros::delay(500);
	robor.chassis.brake();
	robor.move(.819,2.2,1.6,true);
	robor.turn_with_precision(.72, 2.508);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(.72,2.508,.75);
	robor.chassis.move_voltage(volts(1.5));
	pros::delay(2000);
	robor.chassis.move_voltage(volts(-3));
	pros::delay(500);
	robor.chassis.brake();
	pneumatics.set_loader(false);
	robor.move(.7,1.955,1.15,false,true);
	intake.max();
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

void test_mp(){
	timer.reset();
	while(true){
		double voltage = timer.get_elapsed_time().in(milli(seconds)) * 2.0;
		robor.chassis.move_voltage(milli(volts)(voltage));
		std::cout << timer.get_elapsed_time().in(milli(seconds)) << "," << voltage << "," << robor.chassis.forward_motor_displacement().in(meters) << "," << robor.chassis.forward_motor_velocity().in(meters_per_second) << std::endl;
		pros::delay(20);
	}
}

void test_trapezoid(){
	robor.move_feedforward(.25, 1.6);
}

void autonomous() { // all coords are in meters btw
	//test_trapezoid(); // runs the auton that is selected on the gui
	skills();
	
}

bool nanner = false;
bool loader = false;
bool descore = false;

void opcontrol() {
	pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

	robor.chassis.left_motors.raw.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);
	robor.chassis.right_motors.raw.set_brake_mode_all(pros::E_MOTOR_BRAKE_BRAKE);

	while (true) {
		
		// arcade implementation
		robor.chassis.arcade(master.get_analog(ANALOG_LEFT_Y),-master.get_analog(ANALOG_RIGHT_X) * .75);

		if(master.get_digital_new_press(DIGITAL_X)){
			nanner = !nanner;
			pneumatics.set_nanner(nanner);
		}

		if(master.get_digital_new_press(DIGITAL_A)){
			loader = !loader;
			pneumatics.set_loader(loader);
		}

		if(master.get_digital_new_press(DIGITAL_Y)){
			descore = !descore;
			pneumatics.set_descore(descore);
		}


		// intake control
		if(master.get_digital(DIGITAL_R2)){
			if(nanner){
				robor.intake.bottom_max_back_rev();
			}
			else{
				robor.intake.bottom_max();
			}
		}
		else if(master.get_digital(DIGITAL_R1)){
			robor.intake.max();
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