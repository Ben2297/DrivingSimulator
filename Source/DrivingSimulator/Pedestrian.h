#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pedestrian.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DRIVINGSIMULATOR_API APedestrian : public ACharacter
{
	GENERATED_BODY()

	/** Spring arm that will offset the camera */
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
	
	/** Camera component that will be our viewpoint */
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

public:
	// Sets default values for this character's properties
	APedestrian();

	UPROPERTY(EditAnywhere, Category = Movement)
	float PitchValue;

	UPROPERTY(EditAnywhere, Category = Movement)
	float YawValue;

	UPROPERTY(EditAnywhere, Category = Movement)
	float RollValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool running;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handle pressing forwards */
	void WalkForward(float Val);

	/** Handle pressing right */
	void WalkRight(float Val);

	// Handle left/right rotation
	void RotateRight(float Val);

	void setRunning();
};
