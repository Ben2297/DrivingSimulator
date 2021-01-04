#include "AIVehicle.h"
#include "AIVehicleMovementComponent.h"
#include "DrivingSimulatorWheelFront.h"
#include "DrivingSimulatorWheelRear.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"

AAIVehicle::AAIVehicle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UAIVehicleMovementComponent>(AWheeledVehicle::VehicleMovementComponentName))
{
    // Car mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/PolygonTown/Meshes/Vehicles/SK_Veh_Convertable_01.SK_Veh_Convertable_01"));
    GetMesh()->SetSkeletalMesh(CarMesh.Object);

    // Car animation BP
    static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Car/ConvertibleAnimBlueprint"));
    GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

    // Simulation
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
}

void AAIVehicle::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}