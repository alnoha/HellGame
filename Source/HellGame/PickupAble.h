// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "APickUpComp.h"
#include "PickupAble.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupAble : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class HELLGAME_API IPickupAble
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
		void OnPickUp(AActor* Caller, FVector HitLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
		void OnDropPickUp(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
		void OnGrounded(AActor* Caller);
};
