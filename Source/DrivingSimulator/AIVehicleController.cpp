#include "AIVehicleController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIVehicle.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "NavigationSystem.h"

AAIVehicleController::AAIVehicleController()
{
    // Sets Vehicle to call tick function every frame
    PrimaryActorTick.bCanEverTick = true;

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    SightConfig->SightRadius = AISightRange;
    SightConfig->LoseSightRadius = AILoseSightRange;
    SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
    SightConfig->SetMaxAge(AISightAge);

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIVehicleController::OnPawnDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIVehicleController::BeginPlay()
{
    Super::BeginPlay();
}

void AAIVehicleController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AAIVehicleController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AAIVehicleController::MoveForward(float Val)
{
    AAIVehicle* Vehicle = Cast<AAIVehicle>(GetPawn());
    Vehicle->GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void AAIVehicleController::MoveRight(float Val)
{
    AAIVehicle* Vehicle = Cast<AAIVehicle>(GetPawn());
    Vehicle->GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AAIVehicleController::OnHandbrakePressed()
{
    AAIVehicle* Vehicle = Cast<AAIVehicle>(GetPawn());
    Vehicle->GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AAIVehicleController::OnHandbrakeReleased()
{
    AAIVehicle* Vehicle = Cast<AAIVehicle>(GetPawn());
    Vehicle->GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AAIVehicleController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
    
}
