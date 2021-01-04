#include "AIPedestrian.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AAIPedestrian::AAIPedestrian()
{
 	// Sets Vehicle to call tick function every frame
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 220.0f;
}

// Called when the game starts or when spawned
void AAIPedestrian::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAIPedestrian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAIPedestrian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

