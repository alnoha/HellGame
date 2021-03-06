// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "PickupAble.h"
#include "HellGameHUD.h"
#include "HellGameCharacter.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

UCLASS()
class HELLGAME_API AInteractableBase : public AActor, public IInteractable, public IPickupAble
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	AHellGameHUD* HUD;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnInteract(AActor* Caller, FVector ImpactComponent);
	virtual void OnInteract_Implementation(AActor* Caller, FVector ImpactComponent);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnBeginFocus();
	virtual void OnBeginFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnEndFocus();
	virtual void OnEndFocus_Implementation();

	UPROPERTY(BlueprintReadWrite, Category = "Interactable")
		bool bIsFocused;

	UPROPERTY(EditAnywhere, Category = "Interactable")
		TSubclassOf<UUserWidget> PromptWidget;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
		void OnPickUp(AActor* Caller, FVector HitLocation);
	virtual void OnPickUp_Implementation(AActor* Caller, FVector HitLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
		void OnDropPickUp(AActor* Caller);
	virtual void OnDropPickUp_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
		void OnGrounded(AActor* Caller);
		void OnGrounded_Implementation(AActor* Caller);

	/*Can the player pickup this object, if false = stationary*/
	UPROPERTY(EditAnywhere, Category = "Interactable")
		bool bCanPickup;

	AHellGameCharacter* Interactor;
};
