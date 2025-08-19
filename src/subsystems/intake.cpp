#include "subsystems/intake.hpp"

Intake::Intake(
    int8_t motor_port,
    int8_t bottom_motor,
    uint8_t optical_port
) : intake_motor(motor_port), intake_motor_2(bottom_motor), optical_sensor(optical_port) 
{   
    // initialize the stuff (can be changed during runtime)
    intake_motor.set_gearing(pros::E_MOTOR_GEAR_BLUE);
    intake_motor_2.set_gearing(pros::E_MOTOR_GEAR_BLUE);
    optical_sensor.set_led_pwm(10);
};

void Intake::set_alliance(Alliance alliance){
    this->alliance = alliance;
}

void Intake::move(uint8_t power){
    intake_motor.move(power);
    intake_motor_2.move(power);
}

void Intake::move_voltage(ushort voltage){
    intake_motor.move_voltage(voltage);
}

void Intake::max(void){
    intake_motor.move_voltage(12000);
    intake_motor_2.move_voltage(12000);
}

void Intake::bottom_max_top_rev(void){
    intake_motor.move_voltage(3000);
    intake_motor_2.move_voltage(12000);
}

void Intake::bottom_rev(void){
    intake_motor_2.move_voltage(12000);
}

void Intake::reverse(void){
    intake_motor.move_voltage(-12000);
    intake_motor_2.move_voltage(-12000);
}

void Intake::stop(void){
    intake_motor.brake();
    intake_motor_2.brake();
}

void Intake::toggle_color_sort(void){
    do_sort = !do_sort;
}

void Intake::toggle_direction(void){
    direction *= -1;

    //forward: 1 reverse: -1
}