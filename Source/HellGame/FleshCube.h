// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FleshCubeSideBase.h"
#include "SideTypes.h"
#include "InteractableBase.h"
#include "CubeFaceData.h"
#include "Components/BoxComponent.h"
#include "FleshCube.generated.h"


UCLASS(BlueprintType)
class UCubeFaseData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube")
	AFleshCube* ConnectedCube;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube")
	UFleshCubeSideBase* ConnectedFace;
};

UCLASS()
class HELLGAME_API AFleshCube : public AInteractableBase
{
	GENERATED_BODY()

private:
	TArray<UFleshCubeSideBase*> ActivatedSides;

	ESideType PreviousLeftSide;
	ESideType PreviousFrontSide;
	ESideType PreviousRightSide;
	ESideType PreviousBackSide;

	bool bStartSidesGenerated = false;
	bool bCurrentlyCarried = false;
	bool bCanSendStartSignal = false;

public:

	UPROPERTY(EditAnywhere)
	UCubeFaceData* FaceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Respawn")
	AActor* CubeRespawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides")
	ESideType LeftSideType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides")
	ESideType FrontSideType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides")
	ESideType RightSideType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides")
	ESideType BackSideType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	USkeletalMeshComponent* LeftSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	USkeletalMeshComponent* RightSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	USkeletalMeshComponent* FrontSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	USkeletalMeshComponent* BackSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	USkeletalMeshComponent* TopSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	USkeletalMeshComponent* BottomSideMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stationary Mesh")
	USkeletalMesh* BoltedMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stationary Mesh")
	UMaterial* BoltedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* LeftSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* FrontSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* RightSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* BackSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace")
	float CubeSideTraceDistance = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace")
	float CubeGroundTraceDistance = 80.0f;

	/*How far the cube will snap to the other cube. (Default = 250)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace")
	float CubeSnapDistance = 250.0f;

	UFUNCTION()
	void SendActivationSignal(AFleshCube* SendingCube, UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType, bool ReturnSignal = false);


private:
	void SetupBaseMesh();
	void SetupSideMeshes();
	void SetupSideMesh(USkeletalMeshComponent*& MeshComponent, UStaticMeshComponent* ComponentParent, FVector ComponentLocation, FRotator ComponentRotation, FName ComponentName);
	void SetupSides();
	void SetupStartSides();
	void TryToFindCubeNeighbour(FHitResult& CubeHitResult, USkeletalMeshComponent* MeshComponent, FCollisionQueryParams& CollisionParams, UFleshCubeSideBase* SendingSide, ESideType SideType);
	void SetupSide(USkeletalMeshComponent*& SideMeshComponent, ESideType& SideType, ESideType& PreviousType, UFleshCubeSideBase*& CubeSide);
	void LatchCube(FVector Start, UPrimitiveComponent* CubeSide);
	bool bHasLatched;

protected:
	virtual void BeginPlay() override;

	virtual void OnPickUp_Implementation(AActor* Caller, FVector ImpactPoint) override;
	virtual void OnDropPickUp_Implementation(AActor* Caller) override;

public:
	AFleshCube();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Cube sides")
	UFleshCubeSideBase* GetCubeSideByComponentName(FString Name);
	UFUNCTION(BlueprintCallable, Category = "Cube Activation")
	void ReceiveRemoteActivationSignal(FString ColliderName);
};