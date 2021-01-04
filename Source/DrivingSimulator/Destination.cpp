#include "Destination.h"
#include "DrivingSimulatorPawn.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ADestination::ADestination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestination::OnPlayerEnter);

	point1.Coords = FVector(-4540.0f,-11690.0f,270.0f);
	point1.Next = &point2;

	point2.Coords = FVector(5120.0f, -11690.0f, 270.0f);
	point2.Next = &point3;

	point3.Coords = FVector(5120.0f, 8090.0f, 270.0f);
	point3.Next = &point4;

	point4.Coords = FVector(-4540.0f, 8090.0f, 270.0f);
	point4.Next = &point1;

	NextPoint = &point2;
}

// Called when the game starts or when spawned
void ADestination::BeginPlay()
{
	Super::BeginPlay();

	if (ActorOwner != nullptr)
	{
		SetOwner(ActorOwner);
	}
}

// Called every frame
void ADestination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestination::OnPlayerEnter(UPrimitiveComponent* OverlapComponent,
								 AActor* OtherActor,
								 UPrimitiveComponent* OtherComponent,
								 int32 OtherBodyIndex,
								 bool bFromSweep,
								 const FHitResult& SweepResult)
{
	ADrivingSimulatorPawn* DrivingSimulatorPawn = nullptr;

	if(OtherActor != nullptr)
	{
		DrivingSimulatorPawn = Cast<ADrivingSimulatorPawn>(OtherActor);
		if(DrivingSimulatorPawn != nullptr)
		{
			DrivingSimulatorPawn->DriverScore += 100;
			
			Point nextPoint = *NextPoint;
			NextPoint = nextPoint.Next;
			SetActorLocation(nextPoint.Coords);
		}
	}
}

