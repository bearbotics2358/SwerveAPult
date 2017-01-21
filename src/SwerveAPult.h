/*
 * SwerveAPult.h
 *
 *  Created on: Oct 25, 2016
 *      Author: Teacher
 */

#ifndef SRC_SWERVEAPULT_H_
#define SRC_SWERVEAPULT_H_

#include <WPILib.h>
#include <SerialPort.h>
#include "JrimmyGyro.h"
#include "PivotArm.h"
#include "Roller.h"
#include "Lifter.h"
#include "Shooter.h"
#include "SwerveModule.h"
#include "SwerveDrive.h"
#include "LightRingController.h"
#include "SmartDashboard/SmartDashboard.h"
#include "SmartDashboard/SendableChooser.h"

class SwerveAPult : public IterativeRobot {
public:
	SwerveAPult(void);
	void RobotInit(void);
	void DisabledInit(void);
	void DisabledPeriodic(void);
	void TeleopInit(void);
	void TeleopPeriodic(void);
	void TestInit(void);
	void TestPeriodic(void);
	void AutonomousInit(void);
	void AutonomousPeriodicFull(void);
	void AutonomousPeriodic(void);
	void AutonomousPeriodicSimple(void);

private:
	Joystick a_Joystick;
	Joystick a_Joystick2;

	PowerDistributionPanel a_PDP;

	Compressor a_Compressor;

	SwerveModule a_FrontRight;
	SwerveModule a_FrontLeft;
	SwerveModule a_BackLeft;
	SwerveModule a_BackRight;

	SwerveDrive a_Drive;

	PivotArm a_Collector;

	Shooter a_Shooter;

	Roller a_Roller;

	JrimmyGyro a_Gyro;

	LightRingController a_LRC;

	double tState;
	int shooterCurrent;
	int shooterStart;

};




#endif /* SRC_SWERVEAPULT_H_ */
