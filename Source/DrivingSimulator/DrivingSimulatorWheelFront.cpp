#include "DrivingSimulatorWheelFront.h"

UDrivingSimulatorWheelFront::UDrivingSimulatorWheelFront()
{
	ShapeRadius = 40.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 50.f;
	TireConfig = CreateDefaultSubobject<UTireConfig>(TEXT("ConvertibleTireFront"));
	TireConfig->SetFrictionScale(3.0f);
}
