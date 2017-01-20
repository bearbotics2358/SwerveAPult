/*
 * SwerveModule.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: hstechclub
 */

#include "SwerveModule.h"

const double ANALOG_CONV_FACTOR = 1024.0 / 360.0;
const double QUAD_SPEED_CONV_FACTOR = 4 * 40 * 4248 /60 / 10 ; //situational upon bot/ drive type- max rpm is 0 until we have an actual value
// 4 * encoder cpr * max rpm * 1 minute / 60s / 10 = maximum encoder value delta / .1s
SwerveModule::SwerveModule(uint32_t turnMotorPort, uint32_t driveMotorPort)
: a_TurnMotor(turnMotorPort),
  a_DriveMotor(driveMotorPort)
{
	a_TurnMotor.SetControlMode(CANTalon::kPosition);
	a_TurnMotor.SetFeedbackDevice(CANTalon::AnalogEncoder);
	a_TurnMotor.SetSensorDirection(false);
	// a_TurnMotor.SetAnalogPosition(0);
	a_TurnMotor.SetP(0);
	a_TurnMotor.SetI(0);
	a_TurnMotor.SetD(0);
	a_TurnMotor.Set(0);


	a_DriveMotor.SetControlMode(CANTalon::kSpeed);
	a_DriveMotor.SetFeedbackDevice(CANTalon::QuadEncoder);
	a_DriveMotor.SetSensorDirection(false);
	a_DriveMotor.SetP(1);
	a_DriveMotor.SetI(0);
	a_DriveMotor.SetD(0);
	a_DriveMotor.SetF(0);
	a_DriveMotor.Set(0);

}

void SwerveModule::Set(float angle, float speed, float offset)
{
	/*
	if(fabs(angle - GetAngle()) > 180 && angle > 180) {
		angle -= 180;
		speed *= -1.0;
	} else if(fabs(angle - GetAngle()) > 180 && angle < 180) {
		angle += 180;
		speed *= -1.0;
	}
	*/

	a_TurnMotor.Set((angle + offset) * ANALOG_CONV_FACTOR);
	a_DriveMotor.Set(speed * QUAD_SPEED_CONV_FACTOR); // argument is in encoder value delta / .1s- speed is a percentage of maximum wheel speed
}

float SwerveModule::GetAngle()
{
	return a_TurnMotor.GetPosition() / ANALOG_CONV_FACTOR;
}

float SwerveModule::GetSpeed()
{
	return a_DriveMotor.GetSpeed();
}

float SwerveModule::GetDistance()
{
	return a_DriveMotor.GetEncPosition();
}

void SwerveModule::InvertQuad()
{
	a_DriveMotor.SetSensorDirection(true);
}

void SwerveModule::InvertAnalog()
{
	a_TurnMotor.SetSensorDirection(true);
}

void SwerveModule::InvertDriveMotor()
{
	a_DriveMotor.SetInverted(true);
}

void SwerveModule::SetTurnPID(float turnP, float turnI, float turnD)
{
	a_TurnMotor.SetP(turnP);
	a_TurnMotor.SetI(turnI);
	a_TurnMotor.SetD(turnD);
}

void SwerveModule::SetDrivePIDF(float driveP, float driveI, float driveD, float driveF)
{
	a_DriveMotor.SetP(driveP);
	a_DriveMotor.SetI(driveI);
	a_DriveMotor.SetD(driveD);
	a_DriveMotor.SetF(driveF);
}
