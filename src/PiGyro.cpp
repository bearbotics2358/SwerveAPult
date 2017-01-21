/* PiGyro.cpp - interface to I2C based gyro on Raspberry Pi

	 created 3/27/15 BD
	 updated 4/13/15
	 - appeared to give angles ~4x reality
	 - added print statements and found registers were not being set, all
	 of them return 0
	 - problem was in SetRegByte - it was using 2 single byte writes, 1 for
	 reg, 1 for value
	 - changed to single write - works!
	 - drift is very small now - worst axis (X) was 1 degree after 50 secs 
	 - noticed Init is reading the gyro 100 times - reduce this
	 - noticed 1st reading is 0, 2nd reading is wrong
	 - added 100 msec delay before reading
	 - throw out first reading
	 - reduce number of readings to 10
	 updated 4/15/15
	 - change Reset() to Zero() for zeroing the angles

*/

#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "PiGyro.h"

const uint8_t PiGyro::kPowerMgmRegister;
const uint8_t PiGyro::kDataFormatRegister;
const uint8_t PiGyro::kDataRegister;
constexpr double PiGyro::kGsPerLSB;

/**
 * Constructor.
 *
 * @param port The I2C port the gyro is attached to
 */
// PiGyro::PiGyro(Port port):		I2C(port, JAKE_GYRO_ADDRESS)
PiGyro::PiGyro()
{
	file = 0;
	Open();
	Init();
	// m_i2c = new I2C((I2C::Port)port, kAddress);
	// int ret = Read(0, 1, Buff);

	printf("Gyro Reg 0: %2.2X\n", GetReg0() & 0x00ff);
}

PiGyro::~PiGyro()
{
	// delete m_i2c;
	// m_i2c = NULL;
}

void PiGyro::Open()
{
	char filename[20];
	int addr = 0x68; // 0110 1000 7 bit address
	uint8_t buff[3];
	int ret;
	int i;
	int mode = 1;
	int j;

	sprintf(filename, "/dev/i2c-1");
	if((file = open(filename, O_RDWR)) < 0) {
		printf("Couldn't open %s\n", filename);
		exit(1);
	}

	if(ioctl(file, I2C_SLAVE, addr) < 0) {
		printf("ioctl failed\n");
		exit(2);
	} 
}

void PiGyro::Close()
{
	if(file) {
		close(file);
	}
	file = 0;
}

void PiGyro::WaitForValues()
{
	uint8_t stat;

	do {
		stat = GetRegByte(kIntStatus);
	} while(!(stat & 1));
}

void PiGyro::Init()
{
	lastUpdate = 0.0;
	SetRegByte(kDLPFRegister, 0x1B);
	SetRegByte(kSampleRateDivider, 9);
	SetRegByte(kPowerMgmRegister, 1);
	SetRegByte(kIntCfg, 1);

	Cal();

	// did registers set properly?
	printf("Reg 0: 0x%2.2x\n", GetRegByte(0));
	printf("kDLPFRegister(0x%2.2x): 0x%2.2x = 0x1B?\n", kDLPFRegister, (unsigned int)GetRegByte(kDLPFRegister));
	printf("kSampleRateDivider(0x%2.2x): %d = 9?\n",	kSampleRateDivider, (int)GetRegByte(kSampleRateDivider));
	printf("kPowerMgmRegister(0x%2.2x): %d = 1?\n", kPowerMgmRegister, (int)GetRegByte(kPowerMgmRegister));
	printf("kIntCfg(0x%2.2x): %d = 1?\n", kIntCfg, (int)GetRegByte(kIntCfg));
}

void PiGyro::Cal()
{
	// get gyro drift biases
	int i;
	double tstart = GetTime();
	while(GetTime() < tstart + 0.100) {
		// wait 100 ms for readings 
	}
	
	for(i = 0; i < 3; i++) {
		angleBias[i] = 0;
	}

	// throw out first reading, it is 0
	WaitForValues();
	Update();

	for(int i = 0; i < 10; i++) {
		WaitForValues();
		Update();
		angleBias[0] += XAxis;
		angleBias[1] += YAxis;
		angleBias[2] += ZAxis;

		printf("XAxis: %6.2lf  ", XAxis);
		printf("YAxis: %6.2lf  ", YAxis);
		printf("ZAxis: %6.2lf\n", ZAxis);
	}

	for(i = 0; i < 3; i++) {
		// angleBias[i] /= (100 * 14.375); // 10 readings, divide by scale factor
		angleBias[i] /= 10; // 100 readings
		// Update() already divided by scale factor
		angle[i] = 0;
	}
	printf("Bias read time %6.3lf\n", GetTime() - tstart);
	printf("AngleBias: %6.3lf %6.3lf %6.3lf\n", angleBias[0], angleBias[1], angleBias[2]);
}

uint8_t PiGyro::GetReg0()
{
	return GetRegByte(0);
}

uint8_t PiGyro::GetRegByte(uint8_t regNum)
{
	uint8_t ret;
	uint8_t buff[1];

	buff[0] = regNum;
	ret = write(file, buff, 1);
	if(ret != 1) {
		printf("Write returned wrong size: %d - %s\n", ret, strerror(errno));
		exit(3);
	}

	ret = read(file, buff, 1);
	if(ret != 1) {
		printf("Read returned wrong size: %d - %s\n", ret, strerror(errno));
		exit(3);
	}
	// printf("0x%2.2X\n", buff[0]);

	return buff[0];
}

int16_t PiGyro::GetReg(uint8_t regNum)
{
	int16_t ret;
	uint8_t buff[2];

	buff[0] = regNum;
	ret = write(file, buff, 1);
	if(ret != 1) {
		printf("Write returned wrong size: %d - %s\n", ret, strerror(errno));
		exit(3);
	}

	ret = read(file, buff, 2);
	if(ret != 2) {
		printf("Read returned wrong size: %d - %s\n", ret, strerror(errno));
		exit(3);
	}
	ret = (buff[0] << 8) | buff[1];
	// printf("Reg 0x%2.2X: 0x%2.2X 0x%2.2X (%6.0d)\n", regNum, buff[0], buff[1], ret);

	return (int16_t)ret;
}

void PiGyro::SetRegByte(uint8_t regNum, uint8_t value)
{
	uint8_t ret;
	uint8_t buff[2];

	buff[0] = regNum;
	buff[1] = value;
	ret = write(file, (const void *)buff, 2);
	if(ret != 2) {
		printf("Write returned wrong size: %d - %s\n", ret, strerror(errno));
		exit(3);
	}

	return;
}

double PiGyro::GetTime()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + (double)tv.tv_usec/1.0e6);
}

void PiGyro::Update()
{
	if(lastUpdate == 0.0){
		lastUpdate = GetTime();
		return;
	}
	double time = GetTime();
	double timeDelta = (time - lastUpdate);

	temperature = (double)GetReg(kTempRegister);
	temperature = -13200 - temp;
	temperature = temperature / 280;
	temperature += 35;

	XAxis = GetReg(kDataRegister + kAxis_X);
	XAxis = XAxis / 14.375;

	YAxis = GetReg(kDataRegister + kAxis_Y);
	YAxis = YAxis / 14.375;

	ZAxis = GetReg(kDataRegister + kAxis_Z);
	ZAxis = ZAxis / 14.375;

	angle[0] += ((XAxis - angleBias[0]) * timeDelta);
	angle[1] += ((YAxis - angleBias[1]) * timeDelta);
	angle[2] += ((ZAxis - angleBias[2]) * timeDelta);
	lastUpdate = time;

	for(int i = 0; i < 3; i++) {
		if (angle[i] > 180) {
			angle[i] -= 180;
		}	else if (angle[i] < -180) {
			angle[i] += 180;
		}
	}
}

double PiGyro::GetX()
{
	return XAxis;
}

double PiGyro::GetY()
{
	return YAxis;
}

double PiGyro::GetZ()
{
	return ZAxis;
}

double PiGyro::GetTemp() {
	return temperature;
}

double PiGyro::GetAngle(int xyz)
{
	return angle[xyz];
}

void PiGyro::Zero() {
	for(int i = 0; i < 3; i++) {
		angle[i] = 0;
	}
}
