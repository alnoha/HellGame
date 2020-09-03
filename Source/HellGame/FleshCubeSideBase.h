// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SideTypes.h"
#include "FleshCubeSideBase.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLGAME_API UFleshCubeSideBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFleshCubeSideBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* FaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A <- Sidetype")
	ESideType CurrentSideType;
private:

private:

	void SetupBaseMesh();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceivedActivationSignal(UFleshCubeSideBase* SendingSide, ESideType SendingType, FTransform SideTransform, bool RemoteSignal = false);
	virtual void ReceivedActivationSignal_Implementation(UFleshCubeSideBase* SendingSide, ESideType SendingType, FTransform SideTransform, bool RemoteSignal = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceivedStopSignal();
	virtual void ReceivedStopSignal_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceiveRemoteActivationSignal(FTransform SideTransform);
	virtual void ReceiveRemoteActivationSignal_Implementation(FTransform SideTransform);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupMesh(UStaticMeshComponent* parent);
	UStaticMesh* GetFaceMesh();
	UStaticMeshComponent* GetMeshComponent();

	void SetCurrentSideType(ESideType NewSideType);
	ESideType GetCurrentSideType();
};
