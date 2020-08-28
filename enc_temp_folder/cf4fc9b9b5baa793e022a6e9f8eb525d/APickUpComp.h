// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Misc/MessageDialog.h"
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
		void PickUp(AActor* actor);
	UFUNCTION(BlueprintCallable, Category = "PickUp")
		void Drop();

	UPROPERTY( BlueprintReadWrite)
		USceneComponent* HoldPosition;
	UPROPERTY( BlueprintReadWrite)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Rotation")
		float RotateAmount = 90;
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

	void RotateSetup();
	FRotator AltRotateSetup();
	void Rotate(float deltaTime);
	void ResetHoldingPoint();
	void UpdateHoldItemPosition();
	

	void StartCheck(); 
	void WriteErrorMessage(FString message);
	FRotator OldRotation;
	FRotator NewRotation;
	float LerpTimer = 0.0f;
	bool RotatingObject = false;

	FVector startPosition;
	FRotator startRotation;
	
};
