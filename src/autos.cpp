#include "autos.h"
#include "au/au.hpp"
#include "dlib/hardware/timer.hpp"
#include "subsystems/intake.hpp"

std::atomic<bool> is_auton{false};

void left_elim(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	// ------------------- STEP 1 ------------------- //
	intake.bottom_max();
	robor.move_boomerang(.63,-.174,65,3.25); // Curve into middle blocks
	robor.move(.72,.02,1); // Precisely move into the middle goal
	intake.max();
	anti_jam.store(false);
	pros::delay(300);
	intake.mid_intake();
	anti_jam.store(false);
	pros::delay(700); // Put ~3 blocks into goal (may vary)
	intake.stop();
	// ------------------- STEP 2 ------------------- //
	robor.move_boomerang(0,-.64,110,3.75,true); // Curve approach matchloader
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(-.33,-.635,.55,false,true); // Intake 3 blocks from matchloader
	robor.chassis.move_voltage(volts(2.5));
	pros::delay(300);
	robor.move_boomerang(.172, -.724, 70,3.75,true); // Curve back up and approach goal
	anti_jam.store(false);
	pneumatics.set_loader(false);
	robor.move(.34,-.71,1,false,true); // Precisely approach goal with intake
	anti_jam.store(true);
	intake.max();
	pros::delay(2000); // Put all of the blocks into the goal (should be 4-5)
	intake.stop();
	// ------------------- STEP 3 ------------------- //
	robor.move(.1,-.712,1,true); // Align descore mechanism with goal
	robor.move(.35,-.46,1);
	robor.move_boomerang(.93,-.51,120,3.5); // Curve into goal taking control zone
	robor.chassis.move_voltage(volts(1.25));
}

void left_elim_goal(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	// ------------------- STEP 1 ------------------- //
	intake.bottom_max();
	robor.move_boomerang(.63,-.174,65,3.25); // Curve into middle blocks
	// ------------------- STEP 2 ------------------- //
	robor.move(-.036,-.647,1.4,true);
	pneumatics.set_loader(true);
	robor.move(-.42,-.647,.4);
	robor.chassis.move_voltage(volts(2.5));
	pros::delay(200);
	anti_jam.store(false);
	robor.move_boomerang(.1,-.705,120,3.75,true);
	pneumatics.set_loader(false);
	robor.move(.3,-.72,1.6,false,true);
	intake.max();
	anti_jam.store(true);
	pros::delay(2000);
	robor.move(.041,-.721,1.6,true);
	intake.stop();
	robor.move(.35,-.475);
	robor.move_boomerang(.93,-.525,120,3);
	robor.chassis.move_voltage(volts(.8));
}

void right_elim(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	// ------------------- STEP 1 ------------------- //
	intake.bottom_max();
	robor.move_boomerang(.585,.155,65,3.25); // Curve into mid blocks

	robor.move(.73,-.055,1.6,false,true);
	intake.reverse();
	pros::delay(1750);
	intake.bottom_max();
	

	// ------------------- STEP 2 ------------------- //
	pros::delay(500);
	robor.move(-.060,.67,1.4,true);
	pneumatics.set_loader(true);
	robor.move(-.35,.66,.4,false); // Grab loader blocks
	robor.chassis.move_voltage(volts(3.5));
	pros::delay(250);
	robor.move_boomerang(.126,.71,120,3.75,true);
	anti_jam.store(false);
	pneumatics.set_loader(false);
	robor.move(.318,.695,1,false,true); // Align with goal & put blocks in goal
	anti_jam.store(true);
	intake.max();
	pros::delay(1750);
	robor.move(-.034,.706,1.6,true);
	robor.move(.4,.95);
	robor.move_boomerang(.888,.91,110,3.75);
	
}

void right_elim_goal(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	// ------------------- STEP 1 ------------------- //
	intake.bottom_max();
	robor.move_boomerang(.585,.155,65,3.25); // Curve into mid blocks
	

	// ------------------- STEP 2 ------------------- //
	intake.bottom_max();
	robor.move(-.060,.67,1.4,true);
	pneumatics.set_loader(true);
	robor.move(-.35,.67,.4,false); // Grab loader blocks
	robor.chassis.move_voltage(volts(3.5));
	pros::delay(250);
	robor.move_boomerang(.126,.71,120,3.75,true);
	anti_jam.store(false);
	pneumatics.set_loader(false);
	robor.move(.313,.74,1,false,true); // Align with goal & put blocks in goal
	anti_jam.store(true);
	intake.max();
	pros::delay(3000);
	robor.move(-.034,.706,1.6,true);
	robor.move(.4,1);
	intake.bottom_max();
	robor.move_boomerang(.945,.95,120,3.75);
	
}

void all_in_goal(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	// ---------------- STEP 1 ----------------- //
	intake.bottom_max();
	robor.chassis.move_voltage(volts(4));
	pros::delay(200);
	robor.chassis.brake();
	pros::delay(200);
	robor.move(.2,.5,1.8);
	robor.move(.637,.14,.55); // Approach mid intake blocks & pick them up
	// ---------------- STEP 2 ------------------ //
	robor.move(.071,.750,1.6,true); // Approach loader
	pneumatics.set_loader(true); // Activate matchload mechanism
	robor.move(-.421,.683,.55); // Begin matchloading balls
	robor.chassis.move_voltage(volts(3.5));
	pros::delay(200);
	robor.move_boomerang(-.1,.683,120,3.75,true); // Back up from matchloader
	intake.stop();
	pneumatics.set_loader(false);
	robor.move(.300,.73,1,false,true); // Approach goal with intake
	intake.max();

}

void solo(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	intake.bottom_max_back_rev();
	pneumatics.set_nanner(true);
	robor.move(.641,-.012);
	pneumatics.set_loader(true);
	robor.move(.641,-.3,.4);
	anti_jam.store(false);
	robor.chassis.move_voltage(volts(3.5));
	pros::delay(300);
	robor.move(.641,.4,1.6,true);
	intake.max();
	anti_jam.store(false);
	pros::delay(1750);
	pneumatics.set_nanner(false);
	pneumatics.set_loader(false);
	robor.move(.006,.727,.8);
	robor.move(-.122,.83,1.6,false,true);
	intake.mid_intake();


}

void test_tare(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	robor.move(-.5,0);
	robor.imu.tare_position();
	robor.odom.set_position({meters(0),meters(0),degrees(0)});
	robor.move(.5,0);
}



void skills(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	intake.bottom_max();
	robor.move_boomerang(.645,-0.084,85,3.75);
	pneumatics.set_loader(true);
	robor.move(.64,.15,.4);
	robor.chassis.move_voltage(volts(3));
	pros::delay(1600);
	anti_jam.store(false);
	robor.move_boomerang(.652,-.3,120,3.75,true);
	pneumatics.set_loader(false);
	robor.move(.64,-.49,1,false,true);
	anti_jam.store(true);
	intake.max();
	pros::delay(2500);
	robor.move(.675,-.263,1.4,true);
	intake.bottom_max();
	robor.move(.006,-.75,.8);
	anti_jam.store(false);
	robor.move(.62,-.249,1.6,true);
	robor.move(.612,-.49,1,false,true);
	intake.max();
	pros::delay(1500);
	robor.move(.675,-.263,1.4,true);
	intake.reverse();
	robor.move(.073,-.785,1.4);
	robor.move(.17,-2.2,1);
	robor.move(.56,-2.331,1.4);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(.55,-2.6,.4);
	robor.chassis.move_voltage(volts(3));
	pros::delay(1600);
	anti_jam.store(false);
	robor.move_boomerang(.586,-2.102,120,3.75,true);
	pneumatics.set_loader(false);
	robor.move(.605,-1.978,1,false,true);
	anti_jam.store(true);
	intake.max();
	pros::delay(3500);
	robor.move_boomerang(.52,-2.252,120,3.75,true);
	intake.reverse();
	robor.move(-.937,-1.912,1.4);
	robor.move(-1.575,-2.189,1.4);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(-1.575,-2.4,.4);
	robor.chassis.move_voltage(volts(3));
	pros::delay(1600);
	anti_jam.store(false);
	robor.move_boomerang(-1.563, -1.947,120,3.75,true);
	pneumatics.set_loader(false);
	robor.move(-1.563,-1.840,1,false,true);
	anti_jam.store(true);
	intake.max();
	pros::delay(3500);
	robor.move(-1.563,-2.098,1.4,true);
	intake.reverse();
	robor.move(-.951,-1.55,1.4);
	robor.move(-.955,-.368,1);
	robor.move(-1.38,0,1.4);
	pneumatics.set_loader(true);
	intake.bottom_max();
	robor.move(-1.35,.25,.4);
	robor.chassis.move_voltage(volts(3));
	pros::delay(1600);
	anti_jam.store(false);
	robor.move_boomerang(-1.465, -.155,120,3.75,true);
	pneumatics.set_loader(false);
	robor.move(-1.445,-.315,1.4,false,true);
	anti_jam.store(true);
	intake.max();
	pros::delay(3500);
	robor.move_boomerang(-1.37,.075,120,3.75,true);
	robor.turn(-.559,.265);
	robor.chassis.move_voltage(volts(8));
	pros::delay(1900);
	robor.chassis.brake();
}

void skills_low(Intake& intake, Pneumatics& pneumatics, Robot& robor){
	robor.move_boomerang(.64,-.05,120,3.75);
	intake.bottom_max();
	pneumatics.set_loader(true);
	robor.move(.628,.2,.4);
	robor.chassis.move_voltage(volts(3));
	pros::delay(1600);
	anti_jam.store(false);
	robor.move_boomerang(.925,-.31,120,2,true);
	pneumatics.set_loader(false);
	intake.stop();
	pneumatics.set_descore(true);
	robor.move(.92,-1.147,1.4);
	robor.move_boomerang(.64,-2.24,75,1.25);
	robor.move(.63,-1.91,1);
	intake.max();


}

std::vector<double> timestamps;
std::vector<double> positions;

void test_mp(dlib::Timer& timer, Robot& robor){
	timer.reset();
	std::cout << "timestamp,displacement,velocity" << std::endl;

	double last_position = 0;
	while(true){
		double current_position = robor.imu.get_rotation().in(degrees);
		double voltage = 10000;//timer.get_elapsed_time().in(milli(seconds));
		double time = timer.get_elapsed_time().in(seconds);

		timestamps.push_back(time);
		positions.push_back(robor.imu.get_rotation().in(degrees));

		robor.chassis.turn_voltage(-milli(volts)(voltage));
		double velocity = (current_position - last_position) / 0.05;
		//std::cout << "(" << voltage << "," << velocity << "),";

		std::cout << "(" << timer.get_elapsed_time().in(seconds) << "," << velocity << "),";

		if(voltage == 12000){
			break;
		}
		
		last_position = current_position;

		pros::delay(50);
	}
	robor.chassis.brake();
	int size = timestamps.size();

	for(int i = 1; i <= size; i++){
		
		double velocity = ((positions.at(i+1)) - positions.at(i-1)) / (timestamps.at(i+1) - timestamps.at(i-1));
		std::cout << "velocity: " << velocity << " at " << timestamps.at(i) << std::endl;
	}
}