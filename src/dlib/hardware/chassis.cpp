#include "dlib/hardware/chassis.hpp"
#include "au/au.hpp"
#include "motor_group.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"

namespace dlib {

// chassis.cpp

ChassisConfig::ChassisConfig(
    MotorGroupConfig left_motors_config,
    MotorGroupConfig right_motors_config,
    pros::MotorGearset drive_gearset,
    au::Quantity<au::Rpm, double> total_rpm,
    au::Quantity<au::Meters, double> wheel_diameter
)  :
    left_motors(left_motors_config),
    right_motors(right_motors_config),
    drive_gearset(drive_gearset),
    total_rpm(total_rpm),
    wheel_diameter(wheel_diameter) {

    switch (drive_gearset) {
        case pros::MotorGearset::red: this->base_rpm = au::rpm(100); break;
        case pros::MotorGearset::green: this->base_rpm = au::rpm(200); break;
        case pros::MotorGearset::blue: this->base_rpm = au::rpm(600); break;
        case pros::MotorGearset::invalid: this->base_rpm = au::rpm(0); break;
    }
};

Chassis::Chassis(
    ChassisConfig config
) : 
    left_motors(config.left_motors), 
    right_motors(config.right_motors), 
    drive_gearset(config.drive_gearset),
    base_rpm(config.base_rpm),
    total_rpm(config.total_rpm), 
    wheel_diameter(config.wheel_diameter) {
    

}

void Chassis::initialize() {
    left_motors.raw.tare_position_all();
    right_motors.raw.tare_position_all();

    left_motors.raw.set_encoder_units_all(pros::MotorUnits::rotations);
    right_motors.raw.set_encoder_units_all(pros::MotorUnits::rotations);

    left_motors.raw.set_gearing_all(this->drive_gearset);
    right_motors.raw.set_gearing_all(this->drive_gearset);
}

void Chassis::move(const double power) {
    this->left_motors.move(power);
    this->right_motors.move(power);
}

void Chassis::move_voltage(const au::Quantity<au::Volts, double> voltage) {
    this->left_motors.move_voltage(voltage);
    this->right_motors.move_voltage(voltage);
}

void Chassis::turn(const int32_t power) {
    this->left_motors.move(-power);
    this->right_motors.move(power);
}

void Chassis::turn_voltage(const au::Quantity<au::Volts, double> voltage) {
    this->left_motors.move_voltage(voltage);
    this->right_motors.move_voltage(-voltage);
}

void Chassis::arcade(const int32_t power, const int32_t turn) {
    
    this->left_motors.move((power + turn*.5));
    this->right_motors.move((power - turn*.5)*.91);
}
//meow meow meow meow meow meow meow meow meow meow
void Chassis::brake() {
    this->left_motors.raw.brake();
    this->right_motors.raw.brake();
}

au::Quantity<au::Meters, double> Chassis::revolutions_to_displacement(const au::Quantity<au::Revolutions, double> revolutions) const {
    // TODO: move the motor position -> wheel position conversions to a dedicated kinematics class

    // not sure if there's a unit-safe way to do this
    auto wheel_circumference = this->wheel_diameter.in(au::meters) * M_PI;
    auto linear_distance = 
        revolutions.in(au::revolutions) 
        * wheel_circumference 
        * (this->total_rpm / this->base_rpm);
    
    return au::meters(linear_distance);
}

au::Quantity<au::MetersPerSecond, double> Chassis::rpm_to_velocity(const au::Quantity<au::Rpm, double> rpm) const {
    // TODO: move the motor velocity -> wheel velocity conversions to a dedicated kinematics class
    
    // not sure if there's a unit-safe way to do this
    auto wheel_circumference = this->wheel_diameter.in(au::meters) * M_PI;
    auto linear_velocity = 
        (rpm.in(au::rps) 
        * wheel_circumference 
        * (this->total_rpm / this->base_rpm));
    
    return au::meters_per_second(linear_velocity);
}

au::Quantity<au::Meters, double> Chassis::left_motors_displacement() {
    auto revolutions = this->left_motors.get_position();
    auto displacement = 
        this->revolutions_to_displacement(revolutions);
    
    return displacement;
}

au::Quantity<au::Meters, double> Chassis::right_motors_displacement() {
    auto revolutions = this->right_motors.get_position();
    auto displacement = 
        this->revolutions_to_displacement(revolutions);

    return displacement;
}

au::Quantity<au::Meters, double> Chassis::forward_motor_displacement() {
    return (this->left_motors_displacement() + this->right_motors_displacement()) / 2.0;
}

au::Quantity<au::MetersPerSecond, double> Chassis::left_motors_velocity() {
    auto rpm = this->left_motors.get_velocity();
    auto velocity = 
        this->rpm_to_velocity(rpm);
    
    return velocity;
}

au::Quantity<au::MetersPerSecond, double> Chassis::right_motors_velocity() {
    auto rpm = this->right_motors.get_velocity();
    auto velocity = 
        this->rpm_to_velocity(rpm);

    return velocity;
}

au::Quantity<au::MetersPerSecond, double> Chassis::forward_motor_velocity() {
    return (this->left_motors_velocity() + this->left_motors_velocity()) / 2.0;
}

}