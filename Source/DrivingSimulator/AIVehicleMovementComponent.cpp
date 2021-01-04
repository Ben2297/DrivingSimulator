#include "AIVehicleMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Math/Color.h"

float FPIDController::CalcNewInput(float Error, float Position)
{
    ErrorSum = FMath::Clamp(Error + ErrorSum, ErrorMin, ErrorMax);
    float Input = Error * Proportional + ErrorSum * Integral + Derivative * (LastPosition - Position);
    LastPosition = Position;
    return Input;
}

void UAIVehicleMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    Super::RequestDirectMove(MoveVelocity, bForceMaxSpeed);

    // Gets the current vehicle location
    FVector VehicleLocation = GetOwner()->GetActorLocation();

    // Calculates the vehicle destination
    FVector Destination = VehicleLocation + MoveVelocity * GetWorld()->GetDeltaSeconds();

    // Draws the debugging destination line
    DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Destination, FColor::Blue, false, 1.f, 0, 5.f);

    // Calculates the distance between the vehicle and the destination
    FVector Distance = Destination - VehicleLocation;

    // Calculates the distance between the destination and the initial vehicle location
    FVector InitialDistance = Destination - InitialLocation;

    // Gets the current direction of the vehicle
    FVector VehicleDirection = GetOwner()->GetActorForwardVector();

    //// Throttle controller

    // Calculates dot product of direction and the distance
    float ForwardFactor = FVector::DotProduct(VehicleDirection, Distance.GetSafeNormal());

    // 
    float Error = Distance.Size() / InitialDistance.Size() * FMath::Sign(ForwardFactor);
    float Position = 1 - Error;

    float Input = ThrottleController.CalcNewInput(Error, Position);

    const float TURN_AROUND_FACTOR = bTurningAround ? 0.3f : 0.1f;
    if (ForwardFactor < TURN_AROUND_FACTOR && (bForceMaxSpeed || Distance.Size() > 300.f))
    {
        bTurningAround = true;
        SetThrottleInput(-1.f);
    }
    else
    {
        bTurningAround = false;
        SetThrottleInput(bForceMaxSpeed ? ForwardFactor : Input);
    }

    //// Steering controller

    // 
    float InitialYaw = InitialDistance.Rotation().Yaw - InitialDirection.Rotation().Yaw;
    if (InitialYaw < -180)
    {
        InitialYaw += 360;
    }
    else if (InitialYaw > 180)
    {
        InitialYaw -= 360;
    }
    float CurrentYaw = Distance.Rotation().Yaw - VehicleDirection.Rotation().Yaw;
    if (CurrentYaw < -180)
    {
        CurrentYaw += 360;
    }
    else if (CurrentYaw > 180)
    {
        CurrentYaw -= 360;
    }

    float SteeringPosition = (-CurrentYaw + 180) / 180;
    float SteeringError = 1 - SteeringPosition;

    SteeringInput = SteeringController.CalcNewInput(SteeringError, SteeringPosition);
    if (bTurningAround)
    {
        SetSteeringInput(SteeringError > 0 ? -1.f : 1.f);
    }
    else
    {
        SetSteeringInput(SteeringInput);
    }
    SetHandbrakeInput(false);
}

void UAIVehicleMovementComponent::StopActiveMovement()
{
    Super::StopActiveMovement();
    InitialLocation = GetOwner()->GetActorLocation();
    InitialDirection = GetOwner()->GetActorForwardVector();
    SetHandbrakeInput(true);
    SetThrottleInput(0.f);
}
