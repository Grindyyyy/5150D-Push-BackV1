#include "controls/control_drive.h"

void control_drive(Robot& robor, pros::Controller& master){
    // arcade implementation
	robor.chassis.arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y),-master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * .75);
}