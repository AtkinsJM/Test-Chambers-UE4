// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"

AFloorSwitch::AFloorSwitch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwitchCue = nullptr;
	ActorToToggle = nullptr;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::Activate(AActor* Activator)
{
	Super::Activate(Activator);

	Press();
}

void AFloorSwitch::Press()
{
	if (SwitchCue)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SwitchCue);
	}
	if (Mesh)
	{
		Mesh->AddRelativeLocation(FVector(0, 0, -5.0f));
		if (PressedMaterial)
		{
			Mesh->SetMaterial(0, PressedMaterial);
		}
	}
	if (ActorToToggle)
	{
		ActorToToggle->Destroy();
		ActorToToggle = nullptr;
	}
	bIsActivatable = false;
}