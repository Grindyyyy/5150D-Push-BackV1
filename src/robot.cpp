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

    auto total_time = profile.get_time();

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

        if(elapsed_time >= total_time - 100){
            break;
        }

       // std::cout << elapsed_time << "," << setpoint.velocity.in(meters_per_second) << "," << chassis.forward_motor_velocity().in(meters_per_second) << "\n";

        chassis.move_voltage(ff_voltage+pid_voltage);
        
        pros::delay(20);
    }
    chassis.move_voltage(volts(0));
}

void Robot::turn_absolute(Quantity<Degrees, double> heading, bool precise, double max_time) {
    auto start_time = pros::millis();
    auto elapsed_time = 0;
    auto current_time = pros::millis();
    if(precise){
        precise_angular_pid.reset();
        precise_angular_pid_settler.reset();

        while (!precise_angular_pid_settler.is_settled(precise_angular_pid.get_error(), precise_angular_pid.get_derivative())) {
            current_time = pros::millis();
            elapsed_time = current_time - start_time;

            if(elapsed_time > max_time){
                break;
            }
            auto error = dlib::angular_error(heading, imu.get_rotation());
            auto voltage = precise_angular_pid.update(error, milli(seconds)(20));
            chassis.turn_voltage(-voltage);
            pros::delay(20);
        }
    }
    else{
        angular_pid.reset();
        angular_pid_settler.reset();

        while (!angular_pid_settler.is_settled(angular_pid.get_error(), angular_pid.get_derivative())) {
            current_time = pros::millis();
            elapsed_time = current_time - start_time;

            if(elapsed_time > max_time){
                break;
            }

            auto error = dlib::angular_error(heading, imu.get_rotation());
            auto voltage = angular_pid.update(error, milli(seconds)(20));
            chassis.turn_voltage(-voltage);
            pros::delay(20);
        }
    }
    std::cout << "finished in: " << elapsed_time << " milliseconds" << std::endl;
    chassis.brake();
}

void Robot::turn_absolute(double heading, double max_time) {
    turn_absolute(degrees(heading),false);
}

void Robot::turn_relative(Quantity<Degrees, double> heading, double max_time) {
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

void Robot::turn_relative(double heading, double max_time) {
    turn_relative(degrees(heading));
}

void Robot::turn_precise(Quantity<Degrees, double> heading, double max_time){
    precise_angular_pid.reset();
    precise_angular_pid_settler.reset();

    while (!precise_angular_pid_settler.is_settled(precise_angular_pid.get_error(), precise_angular_pid.get_derivative())) {
        auto error = dlib::angular_error(heading, imu.get_rotation());
        auto voltage = precise_angular_pid.update(error, milli(seconds)(20));
        pros::delay(20);
    }
    chassis.brake();
}

void Robot::turn_feedforward(double heading, double max_velocity){
    auto start_heading = imu.get_rotation();
    dlib::TrapezoidProfile<Degrees> profile {
        degrees_per_second_squared(1000),
        degrees_per_second_squared(1000),
        degrees_per_second(max_velocity),
        degrees(heading-start_heading.in(degrees))
    };

    angular_feedforward_pid.reset();
    angular_feedforward_settler.reset();

    auto elapsed_time = 0;
    auto current_time = pros::millis();
    auto start_time = pros::millis();
    auto last_time = 0;

    auto last_heading = 0;
    auto last_position = 0;
    auto error_g = 0.0;

    while (true) {
        current_time = pros::millis();
        elapsed_time = current_time - start_time;

        auto setpoint = profile.calculate(milli(seconds)(elapsed_time));

        auto current_position = imu.get_rotation();

        auto error = dlib::angular_error(setpoint.position+start_heading, current_position);

        auto pid_voltage = angular_feedforward_pid.update(error, milli(seconds)(20));
       // auto ff_voltage = angular_feedforward.calculate(setpoint.velocity, setpoint.acceleration);
        //auto ff_voltage = angular_feedforward.calculate(degrees_per_second(300),degrees_per_second_squared(0));
        auto ff_voltage = volts(std::copysign(1.45, setpoint.velocity.in(degrees_per_second)) + 0.0160736421342 * setpoint.velocity.in(degrees_per_second) + 0.0038 * setpoint.acceleration.in(degrees_per_second_squared));
        
        auto velocity = (current_position.in(degrees) - last_position) / 0.02;


        chassis.turn_voltage(-(ff_voltage+pid_voltage));

        if(profile.stage(milli(seconds)(elapsed_time)) == dlib::TrapezoidProfileStage::Decelerating){
            //chassis.move_voltage(volts(0));
        }
        if(profile.stage(milli(seconds)(elapsed_time)) == dlib::TrapezoidProfileStage::Done){
            break;
        }
        
        last_position = current_position.in(degrees);
        last_time = current_time;

        pros::delay(20);
    }
    chassis.turn_voltage(volts(0));
    //std::cout << "settled!" << std::endl;
}

void Robot::move_boomerang(double x, double y, double max_move, double kp_turn, bool reverse, double theta, double kp_move){
    auto point = dlib::Vector2d(meters(x),meters(y));
    while(true){
        auto displacement = odom.displacement_to(point,reverse);
        auto heading = odom.angle_to(point,reverse);
        auto angle = dlib::angular_error(heading, imu.get_rotation());

        std::cout << displacement << std::endl;
        std::cout << angle << std::endl;

        auto power = displacement.in(meters) * kp_move;
        auto turn = angle.in(degrees) * kp_turn;

        auto left = power + turn;
        auto right = power - turn;

        if(left > max_move){
            left = max_move;
        }
        if(right > max_move){
            right = max_move;
        }

        chassis.left_motors.move(left);
        chassis.right_motors.move(right);

        if(std::abs(displacement.in(inches)) < 2){
            break;
        }
        pros::delay(20);
    }
    chassis.brake();
    std::cout << "settled!" << std::endl;
}

void Robot::turn_precise(double heading, double max_time) {
    turn_absolute(degrees(heading),true);
}

void Robot::move(double x, double y, double max_velocity, bool reverse, bool precise_turn) {
    auto point = dlib::Vector2d(meters(x),meters(y));
    if(precise_turn)
        turn_with_precision(x,y,reverse);
    else
        turn(x,y,reverse);
    auto displacement = odom.displacement_to(point,reverse);
    move_feedforward(displacement.in(meters),max_velocity);
}

void Robot::turn(double x, double y, bool reverse, double max_time) {
    auto point = dlib::Vector2d(meters(x),meters(y));
    auto heading = odom.angle_to(point, reverse);
    std::cout << "heading: " <<  heading << std::endl;
   // turn_feedforward(heading.in(degrees), 600);
   turn_absolute(heading.in(degrees));
}

void Robot::turn_with_precision(double x, double y, bool reverse, double max_time){
    auto point = dlib::Vector2d(meters(x),meters(y));
    auto heading = odom.angle_to(point,reverse);
    turn_precise(heading.in(degrees), max_time);
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



