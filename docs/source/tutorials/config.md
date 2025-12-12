# 2 - Configuration

  

## Chassis

  

In order to properly configure the chassis, we must know a few things about the robot:

  

* Left Motor Group Ports

* Right Motor Group Ports

* Drivetrain Gearset

* RPM/Gear Ratio (depending on chassis config)

* Wheel Diameter

  

First, we will find our motor ports. Each of our motors can either be forward (positive integer) or reverse (negative integer). Different chassis have different amounts of motors, and dLib allows for any amount of motors on either side.

```cpp
dlib::ChassisConfig chassis_config {

{-12,-13,-1},

{19,18,17},

}
```
Finding the remaining characteristics of the chassis is quite easy:
* To find motor gearset, just find the color of motor that you use in your chassis.

|        Motor Type           | **RPM**         | **PROS Declaration**|
| ----------------- | --------------------- | --------------------- |
|    **Blue**       | 600| pros::MotorGearset::blue|
|    **Green**      | 200| pros::MotorGearset::green|
|    **Red**       | 100| pros::MotorGearset::red|

* To find RPM, take the RPM of your chassis' motor and multiply it by the gear ratio on your robot.

|        Gear Ratio| **Motor RPM**         | **Driven RPM**|
| ----------------- | --------------------- | --------------------- |
|    **36:48**       | 600| 450 (600*(36/48))|
|    **12:60**      | 600|120 (600*(12/60))|
|    **2:1**       | 200| 400 (200*(2/1))|
* You can find your wheel diameter with a ruler, or by visiting the VEX website and finding your exact wheel.

**Ultimately, you have finally created the chassis config, and now you can do a bunch of things with the chassis!**

## IMU
Creating the IMU is pretty simple, all you need to do is provide the IMU's port and an optional scalar to correct IMU drift.
```cpp
dlib::ImuConfig  imu_config {
16, // imu port
1 // optional imu scaling constant
};
```
To find the scaling constant, we developed a test that you can do in order to get it easily:

1. Place the robot flush to the wall and start the code.
2. With the code started, move the robot away from the wall with your hand (it shouldn't matter, but try to keep it straight)
3. Spin the bot around 10 times using your controller (come back to this after reading the driver control tutorial if you need it)
4. Observe the IMU heading after the ten spins (it should be a number close to 3600 degrees)
5. Take the wanted heading and divide it by the observed heading ($`3600/observed `$)
6. The number that you get from this is your scaling constant! Run the test a second time with this constant and see if the number is closer.

**Congrats! You have configured a chassis & an IMU.**