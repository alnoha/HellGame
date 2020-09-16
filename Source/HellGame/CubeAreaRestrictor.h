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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
		bool bIsDissolving;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Blueprint Teleportation")
		void TeleportCube(class AActor* OtherActor, class UPrimitiveComponent* OtherComp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Teleport")
		void Disappear(class AActor* OtherActor, class UPrimitiveComponent* OtherComp);
	void Disappear_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Teleport")
		void Reappear(class AActor* OtherActor, class UPrimitiveComponent* OtherComp);
	void Reappear_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp);
};
