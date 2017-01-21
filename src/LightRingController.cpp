#include <LightRingController.h>
#include <Prefs.h>
#include <WPILib.h>

LightRingController::LightRingController()
	:a_I2C(I2C::kOnboard, 0x4){


}

bool LightRingController::SetColor(int device, int r, int g, int b) {
	uint8_t out[3];
	out[0] = r;
	out[1] = g;
	out[2] = b;
	// return a_I2C.Transaction(out, 3, NULL, 0);
	return a_I2C.WriteBulk(out, 3);
}
bool LightRingController::SetFlash(int device, bool set) {
	if(set)
	{
		return SetColor(device,255,255,255);
	}
	else
	{
		return SetColor(device,0,0,0);
	}
}
