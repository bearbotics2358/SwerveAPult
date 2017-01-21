#include "SwerveAPult.h"
#include "Prefs.h"

#include <WPILib.h>
#include <math.h>
#include <stdio.h>

SwerveAPult::SwerveAPult(void):
		a_Joystick(JOYSTICK_PORT),
		a_Joystick2(JOYSTICKTWO_PORT),
		a_PDP(PDP_PORT),
		a_Compressor(PCM_PORT),
		a_FrontRight(FRONT_RIGHT_TURN, FRONT_RIGHT_MOVE),
		a_FrontLeft(FRONT_LEFT_TURN, FRONT_LEFT_MOVE),
		a_BackLeft(BACK_LEFT_TURN, BACK_LEFT_MOVE),
		a_BackRight(BACK_RIGHT_TURN, BACK_RIGHT_MOVE),
		a_Drive(a_FrontRight, a_FrontLeft, a_BackLeft, a_BackRight, CHASSIS_LENGTH, CHASSIS_WIDTH),
		a_Collector(COLLECTOR, COLLECTOR_ENCODER_PORT, 0, 0), // See above
		a_Shooter(SHOOTER, SHOOTER_ENCODER_PORT),
		a_Roller(ROLLER, ROLLER_SWITCH_PORT),
		a_Gyro(I2C::kMXP),
		a_LRC()
{
	tState = 0;
	shooterStart = -99.0;
	shooterCurrent = 1000000000;
	SmartDashboard::init();
		a_Drive.Init();
		a_BackLeft.InvertDriveMotor();
		// a_BackRight.InvertDriveMotor();
		a_FrontRight.InvertDriveMotor();
		// a_FrontLeft.InvertDriveMotor();
		printf("The values is %b", a_LRC.SetFlash(0,false));
}

void SwerveAPult::RobotInit()
{

}

void SwerveAPult::DisabledInit()
{


}

void SwerveAPult::DisabledPeriodic()
{
	a_Gyro.Update();

	SmartDashboard::PutNumber("Gyro, yum", a_Gyro.GetAngle());

	SmartDashboard::PutNumber("Front Right Speed", a_FrontRight.GetSpeed());
	SmartDashboard::PutNumber("Front Left Speed", a_FrontLeft.GetSpeed());
	SmartDashboard::PutNumber("Back Right Speed", a_BackRight.GetSpeed());
	SmartDashboard::PutNumber("Back Left Speed", a_BackLeft.GetSpeed());

	SmartDashboard::PutNumber("Front Right Angle", a_FrontRight.GetAngle());
		SmartDashboard::PutNumber("Front Left Angle", a_FrontLeft.GetAngle());
		SmartDashboard::PutNumber("Back Right Angle", a_BackRight.GetAngle());
		SmartDashboard::PutNumber("Back Left Angle", a_BackLeft.GetAngle());
}

void SwerveAPult::AutonomousInit()
{

}

void SwerveAPult::AutonomousPeriodic() {

}



void SwerveAPult::TeleopInit()
{
	a_Collector.Init();
	a_Collector.SetAngle(0.0);

	a_BackLeft.SetTurnPID(33,0,150);
	a_BackRight.SetTurnPID(33,0,50);
	a_FrontLeft.SetTurnPID(60,0,75);
	a_FrontRight.SetTurnPID(40,0,75);

	a_BackLeft.SetDrivePIDF(1,0,0,0.6);
	a_BackRight.SetDrivePIDF(1,0,0,0.6);
	a_FrontLeft.SetDrivePIDF(1,0,0,1);
	a_FrontRight.SetDrivePIDF(1,0,0,1);
}

void SwerveAPult::TeleopPeriodic()
{

	a_Gyro.Update();

	a_Drive.Update(a_Joystick, a_Gyro.GetAngle());

	if(a_Joystick2.GetRawButton(1)) {
		a_Shooter.Fire();
	}
	if(a_Joystick2.GetRawButton(2)) {
		a_Shooter.Stop();
	}
	a_Shooter.Update(a_Joystick2);

	a_Collector.Update(a_Joystick, 3, 5, 4, 6, 2);



	if(a_Joystick.GetRawButton(8)) {
		a_Roller.Update(0.25);
	} else if(a_Joystick.GetRawButton(1)) {
		a_Roller.Update(-0.5);
	} else {
		a_Roller.Update(0);
	}

	SmartDashboard::PutNumber("Gyro, yum", a_Gyro.GetAngle());

	SmartDashboard::PutNumber("Front Right Speed", a_FrontRight.GetSpeed());
	SmartDashboard::PutNumber("Front Left Speed", a_FrontLeft.GetSpeed());
	SmartDashboard::PutNumber("Back Right Speed", a_BackRight.GetSpeed());
	SmartDashboard::PutNumber("Back Left Speed", a_BackLeft.GetSpeed());


	SmartDashboard::PutNumber("Front Right Angle", a_FrontRight.GetAngle());
		SmartDashboard::PutNumber("Front Left Angle", a_FrontLeft.GetAngle());
		SmartDashboard::PutNumber("Back Right Angle", a_BackRight.GetAngle());
		SmartDashboard::PutNumber("Back Left Angle", a_BackLeft.GetAngle());

}

void SwerveAPult::TestInit()
{

}

void SwerveAPult::TestPeriodic()
{


}

START_ROBOT_CLASS(SwerveAPult);
