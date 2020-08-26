// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APickUpComp.generated.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLGAME_API UAPickUpComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPickUpComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void PickUp(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void Drop();

	UPROPERTY(EditAnywhere)
		USceneComponent* HoldPosition;
		
private:
	UPhysicsHandleComponent* test;
	UPrimitiveComponent* primitive;
	AActor* HoldItem;
	void UpdateHoldItemPosition();

};
