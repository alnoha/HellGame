// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interact.h"
#include "APickUpComp.h"
#include "HellGameHUD.h"
#include "HellGameCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class AHellGameCharacter : public ACharacter, public IInteract
{
	GENERATED_BODY()

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;


public:
	AHellGameCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** How far away the player will be able to interact whit objects */
	UPROPERTY(EditAnywhere, Category = "Interact")
		float InteractDistance = 250.0f;

	/** Is the character holding an object? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	bool bIsHoldingObject;

	void InteractTrace();

	void HidePrompt();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void OnInteract(AActor* Actor);
	virtual void OnInteract_Implementation(AActor* Actor);

	AActor* Interactable;
	UPrimitiveComponent* ImpactComponent;

	bool bIsDrawingPrompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair Widget")
		UUserWidget* CrosshairWidget;


protected:

	AHellGameHUD* HUD;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UAPickUpComp* PickupComponent;

};

