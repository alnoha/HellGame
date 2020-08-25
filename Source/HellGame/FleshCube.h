// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeSideComponent.h"
#include "SideComponent.h"
#include "FleshCube.generated.h"

class UEyeSideComponent;

UCLASS()
class HELLGAME_API AFleshCube : public AActor
{
	GENERATED_BODY()

private:
	TQueue<UEyeSideComponent*> eyeComponents;

	UPROPERTY(EditAnywhere, Category = "Cube Sides")
	UCubeSideComponent* leftSideComponent;
	UCubeSideComponent* frontSideComponent;
	UCubeSideComponent* rightSideComponent;
	UCubeSideComponent* backSideComponent;
	UCubeSideComponent* topSideComponent;
	UCubeSideComponent* bottomSideComponent;

public:

	// Sets default values for this actor's properties
	AFleshCube();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Flesh Sides")
	TEnumAsByte<ECubeSide> leftSide;
	UPROPERTY(EditAnywhere, Category = "Flesh Sides")
	TEnumAsByte<ECubeSide> frontSide;
	UPROPERTY(EditAnywhere, Category = "Flesh Sides")
	TEnumAsByte<ECubeSide> rightSide;
	UPROPERTY(EditAnywhere, Category = "Flesh Sides")
	TEnumAsByte<ECubeSide> backSide;
	UPROPERTY(EditAnywhere, Category = "Flesh Sides")
	TEnumAsByte<ECubeSide> topSide;
	UPROPERTY(EditAnywhere, Category = "Flesh Sides")
	TEnumAsByte<ECubeSide> bottomSide;

private:
	UCubeSideComponent* PopulateSideSlot(ECubeSide cubeSide);
	void CreateEyeComponentQueue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
