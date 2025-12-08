#include "subsystems/intake.hpp"
#include "atomic"

std::atomic<int> spin_bottom{0};
std::atomic<int> spin_middle{0};
std::atomic<int> spin_top{0};
std::atomic<bool> anti_jam{true};

Intake::Intake(
    int8_t motor_port,
    int8_t bottom_motor,
    int8_t middle_motor_p
) : intake_motor(motor_port), intake_motor_2(bottom_motor), middle_motor(middle_motor_p) 
{   
    // initialize the stuff (can be changed during runtime)
    intake_motor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
    intake_motor_2.set_gearing(pros::E_MOTOR_GEAR_BLUE);
    middle_motor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
};

void Intake::set_alliance(Alliance alliance){
    this->alliance = alliance;
}

void Intake::move(uint8_t power){
}

void Intake::move_voltage(ushort voltage){
    intake_motor.move_voltage(voltage);
}

void Intake::max(void){
    if(spin_bottom != 1){
        spin_bottom.store(1);
    }
    if(spin_middle != 1){
        spin_middle.store(1);
    }
    if(spin_top != 1){
        spin_top.store(1);
    }
    anti_jam.store(true);
}

void Intake::mid_intake(void){
    anti_jam.store(true);
    if(spin_bottom != 1){
        spin_bottom.store(1);
    }
    if(spin_middle != 1){
        spin_middle.store(1);
    }
    if(spin_top != 2){
        spin_top.store(2);
    }
}

void Intake::bottom_max_top_rev(void){
}

void Intake::bottom_max_back_rev(void){
    anti_jam.store(false);
    if(spin_bottom != 1){
        spin_bottom.store(1);
    }
    if(spin_middle != 3){
        spin_middle.store(3);
    }
    if(spin_top != 0){
        spin_top.store(0);
    }
}

void Intake::bottom_max(){
    anti_jam.store(true);
    if(spin_bottom != 1){
        spin_bottom.store(1);
    }
    if(spin_middle != 1){
        spin_middle.store(1);
    }
    if(spin_top != 0){
        spin_top.store(0);
    }
}

void Intake::bottom_max_driver(){
    anti_jam.store(false);
    if(spin_bottom != 1){
        spin_bottom.store(1);
    }
    if(spin_middle != 1){
        spin_middle.store(1);
    }
    if(spin_top != 0){
        spin_top.store(0);
    }
}

void Intake::bottom_rev(void){
}

void Intake::reverse(void){
    anti_jam.store(false);
    if(spin_bottom != 3){
        spin_bottom.store(3);
    }
    if(spin_middle != 3){
        spin_middle.store(3);
    }
    if(spin_top != 3){
        spin_top.store(3);
    }
    
}

void Intake::stop(void){
    
    if(spin_bottom != 0){
        spin_bottom.store(0);
    }
    if(spin_middle != 0){
        spin_middle.store(0);
    }
    if(spin_top != 0){
        spin_top.store(0);
    }
    
    anti_jam.store(true);
}

void Intake::toggle_color_sort(void){
    do_sort = !do_sort;
}

void Intake::toggle_direction(void){
    direction *= -1;

    //forward: 1 reverse: -1
}