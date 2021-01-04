#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIPedestrian.generated.h"

UCLASS()
class DRIVINGSIMULATOR_API AAIPedestrian : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIPedestrian();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWaypoint* NextWaypoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
