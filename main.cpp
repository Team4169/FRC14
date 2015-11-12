#include "WPILib.h"//WPI FRC Lib

//#include "GenericHID.h"

class DefaultRobot: public SimpleRobot {
	RobotDrive *myRobot; //Robot Drive System, needed for robot->driveStation communication
	Joystick *rightStick; //Right joystick, controls right motor
	Joystick *leftStick; //Left joystick, controls left motor
	DriverStation *ds; //Drive station, needed for driveStation->robot communication
	Jaguar * s1;//Shooter motor, Jaguar motor controller
	Jaguar * s2;//Shooter motor, Jaguar motor controller
	Victor * c1;//Collect motor, Victor motor controller
	
	enum// Driver Station jumpers to control program operation
	{
		ARCADE_MODE = 1,// Tank/Arcade jumper is on DS Input 1 (Jumper present is arcade)
		ENABLE_AUTONOMOUS = 2,// Autonomous/Teleop jumper is on DS Input 2 (Jumper present is autonomous)
	} jumpers;

public:	
	DefaultRobot(void) {//Constructor, run on robot enabled
		ds = DriverStation::GetInstance();//Set driveStation
		myRobot = new RobotDrive(1, 2);//Create Robot base
		rightStick = new Joystick(1);//Set rightStick to port 1
		leftStick = new Joystick(2);//Set leftStick to port 2
		
		s1 = new Jaguar(4);//Set shooter motor to port 4
		s2 = new Jaguar(5);//Set shooter motor to port 5
		c1 = new Victor(3);//Set collector motor to port 3
		
		myRobot->SetExpiration(0.005);//Set timeout(?)
	}
	
	~DefaultRobot(){//Destructor, run on robot disable
		/* Deleted pointers */
		delete myRobot;
		delete rightStick;
		delete leftStick;
		delete ds;
	}

	void Autonomous(void) {//Autonomous program, run on autonomous enabled
		myRobot->SetSafetyEnabled(false);
		myRobot->Drive(-1.0f, 0.0f);//Go forward, note motors are hooked up backwards
		Wait(3);//Drive forward for 3 seconds
		myRobot->Drive(0.0f, 0.0f);//Stop robot
	}

	void OperatorControl(void) {
		while (IsOperatorControl()) {
			
				myRobot->TankDrive(leftStick, rightStick);//Start automatic motor control

				/* Hacky shooter */
				if(rightStick->GetTrigger() == true){
					s1->Set(-1.0f, 0);
					s2->Set(1.0f, 0);
				}else{
					s1->Set(0.0, 0);
					s2->Set(0.0f, 0);
				}
				
				/* Shooter Control */
				/*if(rightStick->GetTrigger() == true){//On rightStick trigger 
					
					s1->Set(-1.0f, 0);
					s2->Set(1.0f, 0);
				} else if(rightStick->GetTop() == true){//On topButton press
					
					s1->Set(-0.1f, 0);
					s2->Set(0.1f, 0);
				}else{
					
					s1->Set(0.0f, 0);
					s2->Set(0.0f, 0);
				}*/
				
				/* Collector Control */
				if(leftStick->GetTrigger() == true){//On leftStick trigger
					/* Set collector motor to "collect" */
					c1->Set(-1.0f, 0);
				} else{
					/* If trigger is not pulled stop collector */
					c1->Set(0.0f, 0);
				}
				

			// Set the motor value 
			Wait(0.01);
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);//Start
