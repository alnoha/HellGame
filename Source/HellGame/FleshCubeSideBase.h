// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

private:
	void SetupBaseMesh();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
<<<<<<< HEAD
	// Called every frame
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Testes")
	void TextPrinter();
	void TextPrinter_Implementation();
=======
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceivedActivationSignal(UFleshCubeSideBase* SendingSide, ESideType SendingType, FTransform SideTransform);
	virtual void ReceivedActivationSignal_Implementation(UFleshCubeSideBase* SendingSide, ESideType SendingType, FTransform SideTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceivedStopSignal();
	virtual void ReceivedStopSignal_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceiveRemoteActivationSignal();
	virtual void ReceiveRemoteActivationSignal_Implementation();
>>>>>>> 11309d81c5a51b42a286b528578459c84d47d5c8

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetupMesh(UStaticMeshComponent* parent);
	UStaticMesh* GetFaceMesh();
};
