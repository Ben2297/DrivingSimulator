#include "AIPedestrianController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIPedestrian.h"
#include "AIVehicle.h"
#include "DrivingSimulatorPawn.h"
#include "Waypoint.h"

AAIPedestrianController::AAIPedestrianController()
{
    // Sets character to call tick function every frame
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
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIPedestrianController::OnPawnDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIPedestrianController::BeginPlay()
{
    Super::BeginPlay();
}

void AAIPedestrianController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AAIPedestrianController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AAIPedestrian* Pedestrian = Cast<AAIPedestrian>(GetPawn());
    
    if(Pedestrian->NextWaypoint != nullptr && !IsVehicleDetected)
    {
        MoveToActor(Pedestrian->NextWaypoint, 5.0f);
    }
    else if(IsVehicleDetected)
    {
        Pedestrian->GetCharacterMovement()->StopActiveMovement();
    }
}

FRotator AAIPedestrianController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.0f, 0.0f, 0.0f);
    }

    return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAIPedestrianController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
    IsVehicleDetected = false;
    
    for (int i = 0; i < DetectedPawns.Num(); i++)
    {
        AAIVehicle* AaiVehicle = nullptr;
        AaiVehicle = Cast<AAIVehicle>(DetectedPawns[i]);

        ADrivingSimulatorPawn* DrivingSimulatorPawn = nullptr;
        DrivingSimulatorPawn = Cast<ADrivingSimulatorPawn>(DetectedPawns[i]);

        if(AaiVehicle != nullptr || DrivingSimulatorPawn != nullptr)
        {
            FActorPerceptionBlueprintInfo Info;
            GetPerceptionComponent()->GetActorsPerception(DetectedPawns[i], Info);

            if (Info.LastSensedStimuli.Num() > 0)
            {
                const FAIStimulus Stimulus = Info.LastSensedStimuli[0];
     
                if (Stimulus.WasSuccessfullySensed())
                {
                    IsVehicleDetected = true;
                }
            }
        }
    }
}
