#include "DrivingSimulatorPawn.h"
#include "DrivingSimulatorWheelFront.h"
#include "DrivingSimulatorWheelRear.h"
#include "DrivingSimulatorHud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"

#ifndef HMD_MODULE_INCLUDED
#define HMD_MODULE_INCLUDED 0
#endif

const FName ADrivingSimulatorPawn::LookUpBinding("LookUp");
const FName ADrivingSimulatorPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ADrivingSimulatorPawn::ADrivingSimulatorPawn()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/PolygonTown/Meshes/Vehicles/SK_Veh_Convertable_01.SK_Veh_Convertable_01"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	// Car animation BP
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Car/ConvertibleAnimBlueprint"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Movement setup
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UDrivingSimulatorWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("SM_Veh_Convertable_Wheel_fl");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UDrivingSimulatorWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("SM_Veh_Convertable_Wheel_fr");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UDrivingSimulatorWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("SM_Veh_Convertable_Wheel_rl");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UDrivingSimulatorWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("SM_Veh_Convertable_Wheel_rr");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);

	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	//Setup TextRenderMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	
	UMaterialInterface* Material = TextMaterial.Object;

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetTextMaterial(Material);
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetTextMaterial(Material);
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));	
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f,0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->SetupAttachment(GetMesh());

	// Create text render component for driver score
	DriverScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DriverScore"));
	DriverScoreText->SetTextMaterial(Material);
	DriverScoreText->SetRelativeLocation(FVector(50.0f, -75.0f, 99.0f));
	DriverScoreText->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	DriverScoreText->SetupAttachment(GetMesh());
	DriverScoreText->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	
	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Sets up the destination arrow component
	// DestinationArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DestinationArrow"));
	// DestinationArrow->SetupAttachment(GetMesh());
	// DestinationArrow->SetHiddenInGame(false);
	
	bInReverseGear = false;
	DriverScore = 0;
}

void ADrivingSimulatorPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADrivingSimulatorPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADrivingSimulatorPawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ADrivingSimulatorPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ADrivingSimulatorPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ADrivingSimulatorPawn::OnToggleCamera);
}

void ADrivingSimulatorPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ADrivingSimulatorPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ADrivingSimulatorPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ADrivingSimulatorPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ADrivingSimulatorPawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ADrivingSimulatorPawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || ( bForce == true ))
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
		
		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}

void ADrivingSimulatorPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

	//DestinationArrow->SetRelativeRotation(FRotator(75.0f, 0.0f, 0.0f));
}

void ADrivingSimulatorPawn::BeginPlay()
{
	Super::BeginPlay();

	InCarSpeed->SetVisibility(false);
	InCarGear->SetVisibility(false);
	DriverScoreText->SetVisibility(false);
}

void ADrivingSimulatorPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));
	
	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}

	ScoreDisplayString = FText::Format(LOCTEXT("ScoreFormat", "Score: {0}"), FText::AsNumber(DriverScore));
}

void ADrivingSimulatorPawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr) )
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);
		
		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

#undef LOCTEXT_NAMESPACE
