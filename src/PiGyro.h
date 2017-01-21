#pragma once

#include <stdint.h>

// 7 bit address?
#define GYRO_I2C_ADDRESS 0x68

class PiGyro
{
protected:
	static const uint8_t kPowerMgmRegister = 0x3E;
	static const uint8_t kDataFormatRegister = 0x31;
	static const uint8_t kSampleRateDivider = 0x15;
	static const uint8_t kDLPFRegister = 0x16;
	static const uint8_t kTempRegister = 0x1B;
	static const uint8_t kDataRegister = 0x1D;
	static const uint8_t kIntCfg = 0x17;
	static const uint8_t kIntStatus = 0x1A;
	static constexpr double kGsPerLSB = 0.00390625;
	enum PowerCtlFields {kPowerCtl_Link=0x20, kPowerCtl_AutoSleep=0x10, kPowerCtl_Measure=0x08, kPowerCtl_Sleep=0x04};
	enum DataFormatFields {kDataFormat_SelfTest=0x80, kDataFormat_SPI=0x40, kDataFormat_IntInvert=0x20,
		kDataFormat_FullRes=0x08, kDataFormat_Justify=0x04};

public:
	enum Axes {kAxis_X=0x00, kAxis_Y=0x02, kAxis_Z=0x04};
		double temp;
		double XAxis;
		double YAxis;
		double ZAxis;

public:
	explicit PiGyro();
	virtual ~PiGyro();

	// Gyro interface
	void Open();
	void Close();
	void WaitForValues();
	virtual void Init();
	virtual void Cal();
	uint8_t GetReg0();
	uint8_t GetRegByte(uint8_t regNum);
	virtual int16_t GetReg(uint8_t regNum);
	void SetRegByte(uint8_t regNum, uint8_t value);
	virtual double GetX();
	virtual double GetY();
	virtual double GetZ();
	virtual double GetTemp();
	double GetTime();
	virtual void Update();
	virtual double GetAngle(int xyz);
	void Zero();


protected:
	//I2C* m_i2c;
private:
	int file;
	double temperature;
	double AxisX;
	double AxisY;
	double AxisZ;
	double angle[3];
	double angleBias[3];
	double lastUpdate;

};
