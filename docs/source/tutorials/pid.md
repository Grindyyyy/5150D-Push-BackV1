# 4 - PID Control
## Define PID
A PID is a feedback controller that is extremely popular in VEX robotics.  It takes an input (the error & the derivative of whatever you're measuring) and an output (voltage)
Error is the distance from a setpoint. Voltage is closely proportional to error.
```{important}
To learn more about PID, check out the BLRS post in their wiki: https://wiki.purduesigbots.com/software/control-algorithms/pid-controller
```
## Tuning PID
We have a PID config in dLib, and it looks like this:

```cpp
dlib::PidConfig  linear_pid_config {
{
1,// kp gain
0, // ki gain
5 // kd gain
},
volts(12)
};
```
As you can see, PID has 3 gains that can be tuned to produce a viable result.
* kp = proportional, increase this gain to increase the power of the movement (speed)
* ki = integral, increase this gain to increase voltage over time (I wouldn't recommend it)
* kd = derivative, increase this gain to dampen the end of the run and prevent oscillation

To tune these gains:
* Increase kp until the robot begins oscillating at the end of the movement
* Increase kd until the robot stops oscillating
* Repeat this until the robot doesn't stop oscillating no matter how high kd is
* Decrease kp until the robot moves normally

## Angular PID
The robot class has a ``turn_with_pid`` method that changes with the adjustment of the ``turn_pid_config``.

You can conduct a PID movement in the autonomous function. Here is how to do turn:
```cpp
void autonomous() {
	// Try a movement!
	// X and Y are in inches (check overloads above)
	robot.turn_with_pid(90);
}
```

## Linear PID
The robot class has a ``move_with_pid`` method that changes with the adjustment of the ``move_pid_config``.

You can conduct a PID movement in the autonomous function. Here is how to do turn:
```cpp
void autonomous() {
	// Try a movement!
	// X and Y are in inches (check overloads above)
	robot.move_with_pid(12);
}
```