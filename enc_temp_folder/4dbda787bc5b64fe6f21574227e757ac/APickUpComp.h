// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Misc/MessageDialog.h"
#include "Math/Quat.h"	
#include < cmath >
#include "Kismet/KismetMathLibrary.h"
#include <string> 
#include "Math/UnrealMathUtility.h"
#include "APickUpComp.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
		void PickUp(AActor* actor, FVector ImpactComponent);
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void Drop();



	UPROPERTY(BlueprintReadWrite)
		USceneComponent* HoldPosition;
	UPROPERTY(BlueprintReadWrite)
		UPhysicsHandleComponent* PhysicsHandle;

	UFUNCTION(BlueprintCallable, Category = "Move")
		void MoveForward();
	UFUNCTION(BlueprintCallable, Category = "Move")
		void MoveBack();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float MoveAmount = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float MoveMaxLimit = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float MoveMinLimit = 200;


	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void RotateLeft();
	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void RotateRight();
	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void RotateUp();
	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void RotateDown();
	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void AltRotateLeft();
	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void AltRotateRight();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float RotateAmount = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float AltRotateAmount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float LerpSpeed = 5;

	UPROPERTY(BlueprintReadOnly)
		bool IsHolding = false;

private:

	AActor* HoldActor;
	UPrimitiveComponent* HoldItem;
	UStaticMesh* ItemMesh;
	UStaticMeshComponent* temp;


	AActor* Owner;

	void RotateSetup(FRotator DeltaRotation);
	FRotator AltRotateSetup();
	void Rotate(float deltaTime);
	void ResetHoldingPoint();
	void UpdateHoldItemPosition();
	float AngelsBetweenVectors(FVector Vector1, FVector Vector2, FRotator Rotation);

	float DotProduct();

	void StartCheck();
	void WriteErrorMessage(FString message);
	FRotator OldRotation;
	FRotator NewRotation;
	float LerpTimer = 0.0f;
	bool RotatingObject = false;


	FQuat oldRotationTest;
	FQuat rotationTest;
	FVector startPosition;
	FRotator startRotation;
	FQuat CurrentForward;


	void ConvertRotation();
	float DotProd(FVector vec1, FVector vec2);
	FQuat QuatDifferens(FQuat Quat1, FQuat Quat2);
	float AngelsBetween2DVectors(FVector Vector1, FVector Vector2, FVector ForwardVector);
};