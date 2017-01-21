/* JrimmyGyro.cpp

 */


#include <Prefs.h>
#include "JrimmyGyro.h"
// #include <I2C.h>
#include <WPILib.h>
#include <LiveWindow/LiveWindow.h>

const uint8_t JrimmyGyro::kPowerMgmRegister;
const uint8_t JrimmyGyro::kDataFormatRegister;
const uint8_t JrimmyGyro::kDataRegister;
constexpr double JrimmyGyro::kGsPerLSB;

/**
 * Constructor.
 *
 * @param port The I2C port the gyro is attached to
 */
JrimmyGyro::JrimmyGyro(I2C::Port port)
		:m_I2C(port, (0x68 >> 1))
{
	uint8_t Buff[256];
	lastUpdate = 0;
	// Init();
	//m_i2c = new I2C((I2C::Port)port, kAddress);
	int ret = m_I2C.Read(0, 1, Buff);
	printf("Jake Buff: %2.2X\n", Buff[0] & 0x00ff);

	// Turn on the measurements
	m_I2C.Write(kPowerMgmRegister, kPowerCtl_Measure);
	// Specify the data format to read
	//SetRange(range);

	// HALReport(HALUsageReporting::kResourceType_ADXL345, HALUsageReporting::kJrimmyGyro, 0);
	// LiveWindow::GetInstance()->AddSensor("JrimmyGyro", port, this);
}

JrimmyGyro::~JrimmyGyro()
{
	//delete m_i2c;
	//m_i2c = NULL;
}

void JrimmyGyro::WaitForValues()
{
	uint8_t stat;
	bool result;

	double start = Timer::GetFPGATimestamp();
	double now;

	do {
		result = m_I2C.Read(kIntStatus, 1, &stat);
		now = Timer::GetFPGATimestamp();
	} while(!(stat & 1) && !result && ((now - start) < 0.500));
	// TODO: report errors/timeouts
}

void JrimmyGyro::Init()
{
	lastUpdate = 0;
	m_I2C.Write(kDLPFRegister, 0x1B);
	m_I2C.Write(kSampleRateDivider, 9);
	m_I2C.Write(kPowerMgmRegister, 1);
	m_I2C.Write(kIntCfg, 1);

	Cal();

	// did registers set properly?
	printf("Reg 0: 0x%2.2x\n", GetRegByte(0));
	printf("kDLPFRegister(0x%2.2x): 0x%2.2x = 0x1B?\n", kDLPFRegister, (unsigned int)GetRegByte(kDLPFRegister));
	printf("kSampleRateDivider(0x%2.2x): %d = 9?\n",	kSampleRateDivider, (int)GetRegByte(kSampleRateDivider));
	printf("kPowerMgmRegister(0x%2.2x): %d = 1?\n", kPowerMgmRegister, (int)GetRegByte(kPowerMgmRegister));
	printf("kIntCfg(0x%2.2x): %d = 1?\n", kIntCfg, (int)GetRegByte(kIntCfg));
}

void JrimmyGyro::Cal()
{
	// get gyro drift biases
	int i;
	double tstart = Timer::GetFPGATimestamp();
	while(Timer::GetFPGATimestamp() < tstart + 0.100) {
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
		angleBias[i] /= 10;
		angle[i] = 0;
	}
	printf("Bias read time %6.3lf\n", GetTime() - tstart);
	printf("AngleBias: %6.3lf %6.3lf %6.3lf\n", angleBias[0], angleBias[1], angleBias[2]);
	// SmartDashboard::PutNumber("Angle Bias X", angleBias[0]);
	// SmartDashboard::PutNumber("Angle Bias Y", angleBias[1]);
	// SmartDashboard::PutNumber("Angle Bias Z", angleBias[2]);
}

uint8_t JrimmyGyro::GetReg0()
{
	uint8_t id;
	m_I2C.Read(0, 1, &id);
	// SmartDashboard::PutNumber("Gyro ID", id);

	return id;
}

int16_t JrimmyGyro::GetReg(uint8_t regNum)
{
	uint16_t ret;
	uint8_t buff[2];

	m_I2C.Read(regNum, 2, buff);
	ret = (buff[0] << 8) | buff[1];
	return (int16_t)ret;
}

int8_t JrimmyGyro::GetRegByte(uint8_t regNum)
{
	uint8_t buff[2];

	m_I2C.Read(regNum, 1, buff);
	return buff[0];
}

void JrimmyGyro::Update()
{
	if(lastUpdate == 0){
		lastUpdate = Timer::GetFPGATimestamp();
		return;
	}
	double time = Timer::GetFPGATimestamp();
	double timeDelta = (time - lastUpdate);

	temperature = GetReg(kTempRegister);
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

	/* for(int i = 0; i < 3; i++) {
		if (angle[i] > 360) {
			angle[i] -= 360;
		}	else if (angle[i] < 0) {
			angle[i] += 360;
		}
	 } */
}

double JrimmyGyro::GetX()
{
	return XAxis;
}

double JrimmyGyro::GetY()
{
	return YAxis;
}

double JrimmyGyro::GetZ()
{
	return ZAxis;
}

int JrimmyGyro::GetTemp() {
	return temperature;
}

double JrimmyGyro::GetAngle(int xyz)
{
	return angle[xyz];
}

void JrimmyGyro::Zero() {
	for(int i = 0; i < 3; i++) {
		angle[i] = 0;
	}
}

std::string JrimmyGyro::GetSmartDashboardType() {
	return "3AxisAccelerometer";
}
