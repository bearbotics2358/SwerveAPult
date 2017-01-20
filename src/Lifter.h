/*
 * Lifter.h
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */

#ifndef SRC_LIFTER_H_
#define SRC_LIFTER_H_

#include <CanTalonSRX.h>

class Lifter // This is for a motor that extends an arm, like a tape measure- uses a Quadrature Encoder- lifts the bot
{
public:
	Lifter(int TalonPort, int EncoderAPort, int EncoderBPort, int SwitchPort, int Solenoid1, int Solenoid2);
	~Lifter();
	void Update(uint8_t syncGroup = 0);
	void Set(float value1, float setPoint);
	float GetLength();
	void Set(float value);
	void Disable();
	void PIDWrite(float output);
	void Shift(int Shift);
	void ShiftOpen();
	void ShiftClosed();

private:
	CanTalonSRX LifterC;

	Encoder EncoderC;

	DigitalInput a_LifterSwitch;

	DoubleSolenoid a_PleaseDoNotBreakTheBot; // no promises // challenge accepted
};



#endif /* SRC_LIFTER_H_ */
