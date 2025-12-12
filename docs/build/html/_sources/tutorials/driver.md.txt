# 3 - Driver Control
## Controller
A controller contains two joysticks, both having an x axis and a y axis. Driver control is extremely customizable in dLib, so we can elect to use or not use any of these joysticks and any of these axes.

## Tank Drive Control
Tank control is pretty easy, and works like this:
* Left Joystick Y Axis: Left Motors fwd/reverse
* Right Joystick Y Axis: Right Motors fwd/reverse

The code in dLib would look something like this:
```cpp
void  Chassis::arcade(const int32_t left_power, const int32_t right_power) {
this->left_motors.move(left_power);
this->right_motors.move(right_power);
}
```
You can adjust joystick sensitivity by applying a multiplier to left/right power.
You put this function into the chassis.cpp class so that it can be accessed.

## Arcade Drive Control
Arcade drive control is what the creators of dLib love to use, and as such it is baked into dLib. Arcade works like this:

**2 Joystick Arcade**
* Left Joystick Y Axis: Power (fwd/reverse)
* Right Joystick X Axis: Turn (left/right)
* Left moves with Power + Turn, right moves with Power - Turn

**1 Joystick Arcade**
* Left/Right Joystick Y Axis: Power (fwd/reverse)
* Left/Right Joystick X Axis: Turn (left/right)
* Whichever joystick you choose controls both power and turn with the x and y axes

The code in dLib looks like this:
```cpp
void  Chassis::arcade(const  int32_t  power, const  int32_t  turn) {
this->left_motors.move((power  +  turn));
this->right_motors.move((power  -  turn));
}
```
You have more free reign over customization for joystick sensitivity because you can adjust power and turn separately.
You put this function into the chassis.cpp class so that it can be accessed.

## opcontrol
We need to put our drive control into a loop so that it can update consistently to fit controller values. The default PROS template includes a function opcontrol, and here is what it should look like:

```cpp
void opcontrol(){
	pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);
	while(true){
		int power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		robot.chassis.arcade(power,turn); // or tank(power,turn);
		

		pros::delay(20); // add a delay to prevent lag
	}
	
}
```

Once you have this, give it a spin and see how your bot moves! You may need to adjust your chassis config if the robot behaves incorrectly in driver control.

Congrats! You have created driver control.