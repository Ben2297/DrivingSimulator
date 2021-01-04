#include "DrivingSimulatorGameMode.h"
#include "DrivingSimulatorPawn.h"
#include "DrivingSimulatorHud.h"

ADrivingSimulatorGameMode::ADrivingSimulatorGameMode()
{
	DefaultPawnClass = ADrivingSimulatorPawn::StaticClass();
	HUDClass = ADrivingSimulatorHud::StaticClass();
}
