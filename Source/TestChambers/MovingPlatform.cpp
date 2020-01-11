// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Engine/World.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = Root;
	Root->SetCollisionProfileName("Trigger");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	Mesh->SetupAttachment(Root);

	bIsMoving = false;

	MovementDelay = 1.0f;
	MovementSpeed = 100.0f;

	CurrentWaypointIndex = 0;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	Root->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnBeginOverlap);
	Root->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatform::OnEndOverlap);

	Passenger = nullptr;

	FVector StartPos = GetActorLocation();
	TargetLocation = StartPos;
	WaypointLocations.Add(StartPos);

	for (AActor* Waypoint : Waypoints)
	{
		WaypointLocations.Add(Waypoint->GetActorLocation());
	}

}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		UE_LOG(LogTemp, Warning, TEXT("Moving platform!"));
		UE_LOG(LogTemp, Warning, TEXT("Target location: %s"), *TargetLocation.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Current location: %s"), *GetActorLocation().ToString());
		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, MovementSpeed);
		UE_LOG(LogTemp, Warning, TEXT("New location: %s"), *NewLocation.ToString());
		SetActorLocation(NewLocation);
		Passenger->SetActorLocation(NewLocation + PassengerOffset);
		float Distance = (NewLocation - TargetLocation).Size();
		UE_LOG(LogTemp, Warning, TEXT("Distance from target: %f"), Distance);
		if (Distance < 1.0f)
		{
			SetActorLocation(TargetLocation);
			Passenger->SetActorLocation(TargetLocation + PassengerOffset);
			bIsMoving = false;
			if (Passenger)
			{
				Passenger->SetCanMove(true);
			}
		}
	}
}

void AMovingPlatform::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Platform %s collided with %s!"), *OverlappedComponent->GetName(), *OtherComp->GetName());
	if (bIsMoving) { return; }
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerCharacter) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Player detected!"));

	Passenger = PlayerCharacter;
	Passenger->SetCanMove(false);
	PassengerOffset = Passenger->GetActorLocation() - GetActorLocation();
	GetWorld()->GetTimerManager().SetTimer(PlatformTimerHandle, this, &AMovingPlatform::SetNewDestination, MovementDelay, false);
	
}

void AMovingPlatform::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Platform %s end collision with %s"), *OverlappedComponent->GetName(), *OtherComp->GetName());
	
	if (bIsMoving) { return; }
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerCharacter) { return; }
	if (PlayerCharacter == Passenger)
	{
		Passenger = nullptr;
	}
}

void AMovingPlatform::SetNewDestination()
{
	UE_LOG(LogTemp, Warning, TEXT("Setting new destination for platform!"));
	PassengerOffset = Passenger->GetActorLocation() - GetActorLocation();
	CurrentWaypointIndex = (CurrentWaypointIndex + 1) % WaypointLocations.Num();
	TargetLocation = WaypointLocations[CurrentWaypointIndex];
	bIsMoving = true;
}