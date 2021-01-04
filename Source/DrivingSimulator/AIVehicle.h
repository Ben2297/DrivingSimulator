#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "AIVehicle.generated.h"

UCLASS()
class DRIVINGSIMULATOR_API AAIVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:
	AAIVehicle(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
