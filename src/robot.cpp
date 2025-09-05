#include "robot.hpp"

void Robot::initialize(){
    chassis.initialize();

    chassis.left_motors.raw.set_gearing_all(pros::E_MOTOR_GEAR_BLUE);
    chassis.right_motors.raw.set_gearing_all(pros::E_MOTOR_GEAR_BLUE);

    imu.initialize();
}

void Robot::move_pid(Quantity<Meters, double> displacement) {
    auto start_displacement = chassis.forward_motor_displacement();
    auto target_displacement = dlib::relative_target(start_displacement, displacement);
    
    linear_pid.reset();
    linear_pid_settler.reset();

    while (!linear_pid_settler.is_settled(linear_pid.get_error(), linear_pid.get_derivative())) {
        auto error = dlib::linear_error(target_displacement, chassis.forward_motor_displacement());
        auto voltage = linear_pid.update(error, milli(seconds)(20));
        std::cout << voltage << std::endl;
        chassis.move_voltage(voltage);
        pros::delay(20);
    }
    chassis.brake();
}

void Robot::move_pid(double displacement) {
    move_pid(meters(displacement));
}

void Robot::move_feedforward(double displacement, double max_velocity){
    auto start_displacement = chassis.forward_motor_displacement();
    dlib::TrapezoidProfile<Meters> profile {
        meters_per_second_squared(3),
        meters_per_second_squared(3),
        meters_per_second(max_velocity),
        meters(displacement)
    };

    linear_feedforward_pid.reset();
    linear_pid_settler.reset();

    auto elapsed_time = 0;
    auto current_time = pros::millis();
    auto start_time = pros::millis();

    while (true) {
        current_time = pros::millis();
        elapsed_time = current_time - start_time;

        auto setpoint = profile.calculate(milli(seconds)(elapsed_time));

        auto current_position = chassis.forward_motor_displacement();
        auto target_position = dlib::relative_target(start_displacement, setpoint.position);

        auto error = dlib::linear_error(target_position, current_position);

        auto pid_voltage = linear_feedforward_pid.update(error, milli(seconds)(20));
        auto ff_voltage = linear_feedforward.calculate(setpoint.velocity, setpoint.acceleration);
        
        if(profile.stage(milli(seconds)(elapsed_time)) == dlib::TrapezoidProfileStage::Decelerating){
            chassis.move_voltage(volts(0));
        }

        if(profile.stage(milli(seconds)(elapsed_time)) == dlib::TrapezoidProfileStage::Done){
            break;
        }

        std::cout << elapsed_time << "," << setpoint.velocity.in(meters_per_second) << "," << chassis.forward_motor_velocity().in(meters_per_second) << "\n";

        chassis.move_voltage(ff_voltage + pid_voltage);
        
        pros::delay(20);
    }
    chassis.move_voltage(volts(0));
}

void Robot::turn_absolute(Quantity<Degrees, double> heading) {
    angular_pid.reset();
    angular_pid_settler.reset();

    while (!angular_pid_settler.is_settled(angular_pid.get_error(), angular_pid.get_derivative())) {
        auto error = dlib::angular_error(heading, imu.get_rotation());
        auto voltage = angular_pid.update(error, milli(seconds)(20));
        chassis.turn_voltage(-voltage);
        pros::delay(20);
    }
    chassis.brake();
}

void Robot::turn_absolute(double heading) {
    turn_absolute(degrees(heading));
}

void Robot::turn_relative(Quantity<Degrees, double> heading) {
    auto start_heading = imu.get_rotation();
    auto target_heading = dlib::relative_target(start_heading, heading);

    angular_pid.reset();
    angular_pid_settler.reset();

    while(!angular_pid_settler.is_settled(angular_pid.get_error(), angular_pid.get_derivative())) {
        auto error = dlib::angular_error(target_heading, imu.get_rotation());
        auto voltage = angular_pid.update(error, milli(seconds)(20));
        chassis.turn_voltage(-voltage);
        pros::delay(20);
    }
    chassis.brake();
}

void Robot::turn_relative(double heading) {
    turn_relative(degrees(heading));
}

void Robot::turn_precise(Quantity<Degrees, double> heading) {
    precise_angular_pid.reset();
    precise_angular_pid_settler.reset();

    while (!precise_angular_pid_settler.is_settled(precise_angular_pid.get_error(), precise_angular_pid.get_derivative())) {
        auto error = dlib::angular_error(heading, imu.get_rotation());
        auto voltage = precise_angular_pid.update(error, milli(seconds)(20));
        pros::delay(20);
    }
    chassis.brake();
}

void Robot::turn_precise(double heading) {
    turn_absolute(degrees(heading));
}

void Robot::move(double x, double y, double max_velocity, bool reverse, bool precise_turn) {
    auto point = dlib::Vector2d(meters(x),meters(y));
    if(precise_turn)
        turn_with_precision(x,y,reverse);
    else
        turn(x,y,reverse);
    auto displacement = odom.displacement_to(point);
    if(reverse){
        displacement = -displacement;
    }
    move_pid(displacement.in(meters));
}

void Robot::turn(double x, double y, bool reverse) {
    auto point = dlib::Vector2d(meters(x),meters(y));
    auto heading = odom.angle_to(point, reverse);
    turn_absolute(heading.in(degrees));
}

void Robot::turn_with_precision(double x, double y, bool reverse){
    auto point = dlib::Vector2d(meters(x),meters(y));
    auto heading = odom.angle_to(point,reverse);
    turn_precise(heading.in(degrees));
}

void Robot::start_odom() {
    odometry_updater = std::make_unique<pros::Task>([this]() {
        while (true) {
            odom.update(
                chassis.left_motors_displacement(), 
                chassis.right_motors_displacement(), 
                ZERO,
                imu.get_rotation()
            );

            pros::delay(20);
        }
    });
}



