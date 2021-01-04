#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIPedestrianController.generated.h"

UCLASS()
class DRIVINGSIMULATOR_API AAIPedestrianController : public AAIController
{
	GENERATED_BODY()

public:
	AAIPedestrianController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightRange = 500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AISightAge = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AILoseSightRange = AISightRange + 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIFieldOfView = 70.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool IsVehicleDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer = 0.0f;
};
