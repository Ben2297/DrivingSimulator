#include "DrivingSimulatorWheelRear.h"

UDrivingSimulatorWheelRear::UDrivingSimulatorWheelRear()
{
	ShapeRadius = 40.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
	TireConfig = CreateDefaultSubobject<UTireConfig>(TEXT("ConvertibleTireRear"));
	TireConfig->SetFrictionScale(3.0f);
}
