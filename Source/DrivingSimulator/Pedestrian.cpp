#include "Pedestrian.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APedestrian::APedestrian()
{
 	// Sets character to call tick function every frame
	PrimaryActorTick.bCanEverTick = true;

	// Pedestrian mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PedestrianMesh(TEXT("/Game/PolygonTown/Meshes/Characters/SK_Character_Father_01"));
	GetMesh()->SetSkeletalMesh(PedestrianMesh.Object);

	// Pedestrian animation BP
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Animations/CharacterAnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(5.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.0f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Set initial movement variables
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	running = false;

	bUseControllerRotationYaw = false;

	PitchValue = 0.0f;
	YawValue = 0.0f;
	RollValue = 0.0f;
}

// Called when the game starts or when spawned
void APedestrian::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APedestrian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APedestrian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("WalkForward", this, &APedestrian::WalkForward);
	PlayerInputComponent->BindAxis("WalkRight", this, &APedestrian::WalkRight);
	PlayerInputComponent->BindAxis("RotateRight", this, &APedestrian::RotateRight);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APedestrian::setRunning);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APedestrian::setRunning);
}

void APedestrian::WalkForward(float Val)
{
	FVector Direction = Camera->GetForwardVector();
	
	if (!running)
	{
		AddMovementInput(Direction, Val);
	}
	else
	{
		AddMovementInput(Direction, (Val * 2.0f));
	}
}

void APedestrian::WalkRight(float Val)
{
	FVector Direction = Camera->GetRightVector();
	
	if (!running)
	{
		AddMovementInput(Direction, Val);
	}
	else
	{
		AddMovementInput(Direction, Val * 2.0f);
	}
}

void APedestrian::RotateRight(float Val)
{
	YawValue += Val;
	SpringArm->SetRelativeRotation(FRotator(5.f, YawValue, 0.f));
}


void APedestrian::setRunning()
{
	if (!running)
	{
		running = true;
	}
	else
	{
		running = false;
	}
}

