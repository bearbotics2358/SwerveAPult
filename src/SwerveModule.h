/*
 * SwerveModule.h
 *
 *  Created on: Mar 21, 2016
 *      Author: hstechclub
 */

#pragma once

#include <WPILib.h>
#include <SpeedController.h>
#include <CANTalon.h>

class SwerveModule
{
public:
	SwerveModule(uint32_t turnMotorPort, uint32_t driveMotorPort);
	virtual ~SwerveModule() = default;

	void Set(float angle, float speed, float offset);

	float GetAngle();
	float GetSpeed();
	float GetDistance();

	void InvertQuad();
	void InvertAnalog();

	void InvertDriveMotor();

	void SetTurnPID(float turnP, float turnI, float turnD);
	void SetDrivePIDF(float driveP, float driveI, float driveD, float driveF);

private:
	CANTalon a_TurnMotor;
	CANTalon a_DriveMotor;
};

/* SRC_SWERVEMODULE_H_ */
