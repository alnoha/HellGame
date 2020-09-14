// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FleshCube.h"
#include "CubeAreaRestrictor.generated.h"

UCLASS()
class HELLGAME_API ACubeAreaRestrictor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACubeAreaRestrictor();

public:

	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
