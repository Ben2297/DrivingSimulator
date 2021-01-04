#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIVehicleController.generated.h"

UCLASS()
class DRIVINGSIMULATOR_API AAIVehicleController : public AAIController
{
	GENERATED_BODY()

public: 
	AAIVehicleController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	// Handle pressing forwards
	void MoveForward(float Val);
	
	// Handle pressing right
	void MoveRight(float Val);
	
	// Handle handbrake pressed
	void OnHandbrakePressed();
	
	// Handle handbrake released
	void OnHandbrakeReleased();

	UFUNCTION()
    void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRange = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRange = AISightRange + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;
};
