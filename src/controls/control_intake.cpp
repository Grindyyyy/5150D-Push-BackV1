#include "controls/control_intake.h"
#include "controls/control_pneumatics.h"
#include "subsystems/intake.hpp"

std::atomic<bool> jam{false};

void control_intake(Intake& intake, pros::Controller& master){
    // intake control
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        if(nanner){
            intake.bottom_max_back_rev();
        }
        else{
            intake.bottom_max_driver();
        }
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intake.max();
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake.mid_intake();
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake.reverse();
    }
    else{
        intake.stop();
    }
}

double last_torque = 0;

void control_jam(Intake& intake){
    double curr_torque = intake.intake_motor.get_torque();
    double delta_torque = curr_torque-last_torque;

    if(delta_torque > 0.1){
        std::cout << intake.intake_motor.get_torque() << std::endl;
    }

    if(anti_jam){
        if(intake.intake_motor.get_torque() > 0.4 && std::abs(intake.intake_motor.get_actual_velocity()) < 15){
            jam.store(true);
            intake.intake_motor.move_voltage(-12000);
            intake.intake_motor_2.move_voltage(-12000);
            intake.middle_motor.move_voltage(-12000);

            
            pros::delay(350);
            jam.store(false);
            pros::delay(400);
        }
        if(intake.intake_motor_2.get_torque() > 0.3 && std::abs(intake.intake_motor_2.get_actual_velocity()) < 15){
            jam.store(true);
            intake.middle_motor.move_voltage(-12000);
            intake.intake_motor_2.move_voltage(-12000);
            pros::delay(350);
            jam.store(false);
            pros::delay(400);
        }
    }
}

void control_intake_task(Intake& intake){
    if(!jam){
        switch(spin_bottom) { 
            case(1): intake.intake_motor_2.move_voltage(12000); break;
            case(2): intake.intake_motor_2.move_voltage(4000); break;
            case(3): intake.intake_motor_2.move_voltage(-12000); break;
            case(0): intake.intake_motor_2.move_voltage(0); break;
            default: break;
                
        }
        switch(spin_middle) { 
            case(1): intake.middle_motor.move_voltage(12000); break;
            case(2): intake.middle_motor.move_voltage(4000); break;
            case(3): intake.middle_motor.move_voltage(-12000); break;
            case(0): intake.middle_motor.move_voltage(0); break;
            default: break;
        }
        switch(spin_top) { 
            case(1): intake.intake_motor.move_voltage(12000); break;
            case(2): intake.intake_motor.move_voltage(4000); break;
            case(3): intake.intake_motor.move_voltage(-12000); break;
            case(0): intake.intake_motor.move_voltage(0); break;
            default: break;
            
        }
    }
   
}