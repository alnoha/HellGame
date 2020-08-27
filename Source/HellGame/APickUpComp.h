// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "APickUpComp.generated.h"


UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HELLGAME_API UAPickUpComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAPickUpComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void PickUp(UPrimitiveComponent* actor);
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void Drop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* HoldPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPhysicsHandleComponent* test;

	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void RotateLeft();
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void RotateRight();
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void RotateUp();
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void RotateDown();
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void AltRotateLeft();
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void AltRotateRight();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotateAmount = 90;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AltRotateAmount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float lerpSpeed = 5;

private:
	UPrimitiveComponent* HoldItem;
	AActor* Owner;
	void RotateSetup();
	FRotator AltRotateSetup();
	void Rotate(float deltaTime);
	void ResetHoldingPoint();
	void UpdateHoldItemPosition();
	
	FRotator OldRotation;
	FRotator NewRotation;
	float lerpTimer = 0.0f;
	bool RotatingObject = false;

	FVector startPosition;
	FRotator startRotation;
	
};
