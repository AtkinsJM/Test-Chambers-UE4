// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class TESTCHAMBERS_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerCharacterController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TSubclassOf<class ACameraActor> FollowCameraClass;

	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* Pawn) override;

	virtual void OnUnPossess() override;

public:

	void MoveForward(float Value);
	void MoveSideways(float Value);
	
	void AssignCamera();

	void Interact();

	void ReturnToMenu();

private:
	bool bIsRolling;
	
	class AFollowCamera* FollowCamera;

	class UTestChambersGameInstance* GameInstance;
};
