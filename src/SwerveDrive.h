/*
 * SwerveDrive.h
 *
 *  Created on: Mar 22, 2016
 *      Author: hstechclub
 */

#pragma once

#include <WPILib.h>

#include "Prefs.h"
#include "SwerveModule.h"

#include <math.h>
#include "SmartDashboard/SmartDashboard.h"
#include "SmartDashboard/SendableChooser.h"
#include "SmartDashboard/NamedSendable.h"
#include "SmartDashboard/SendableChooser.inc"
#include "SmartDashboard/SendableChooserBase.h"
#include "SmartDashboard/Sendable.h"
#include "Joystick.h"

class SwerveDrive {
public:
	SwerveDrive(SwerveModule &FR, SwerveModule &FL, SwerveModule &BL, SwerveModule &BR, float Length, float Width);
	virtual ~SwerveDrive() = default;

	void Init();
	void InitSendableChooser();

	void Update(Joystick &stick, float gyroValue);
	void SetTwistingMode();
	void DisableTwist();
	void SetTwistingRelAngle(float gyroAngle, float angle);

private:
	static const std::string CONTROL_TYPE_KEY;

	static const int         CONTROL_TYPE_SWERVE;
	static const std::string CONTROL_TYPE_SWERVE_KEY;
	static const int         CONTROL_TYPE_CRAB;
	static const std::string CONTROL_TYPE_CRAB_KEY;
	static const int         CONTROL_TYPE_SIMPLE_DRIVE;
	static const std::string CONTROL_TYPE_SIMPLE_DRIVE_KEY;
	static const int         CONTROL_TYPE_TURNING_DRIVE;
	static const std::string CONTROL_TYPE_TURNING_DRIVE_KEY;

	SwerveModule &a_FrontRight; // "1"
	SwerveModule &a_FrontLeft; // "2"
	SwerveModule &a_BackLeft; // "3"
	SwerveModule &a_BackRight; // "4"

	float a_RobotLength;
	float a_RobotWidth;
	float a_ChassisRadius;

	frc::SendableChooser<std::string> a_ControlTypeChooser;
};


/* SRC_SWERVEDRIVE_H_ */
