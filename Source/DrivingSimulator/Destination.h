#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Destination.generated.h"

UCLASS()
class DRIVINGSIMULATOR_API ADestination : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestination();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* ActorOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	struct Point
	{
		FVector Coords = {0.0f, 0.0f, 0.0f};
		struct Point *Next = nullptr;
	};

	TArray<Point> Points;

	Point *NextPoint;

	Point point1;
	Point point2;
	Point point3;
	Point point4;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    void OnPlayerEnter(UPrimitiveComponent* OverlapComponent,
                        AActor* OtherActor,
                        UPrimitiveComponent* OtherComponent,
                        int32 OtherBodyIndex,
                        bool bFromSweep,
                        const FHitResult &SweepResult);
};
