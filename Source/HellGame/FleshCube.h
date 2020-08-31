// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FleshCubeSideBase.h"
#include "SideTypes.h"
#include "InteractableBase.h"
#include "Engine/DataTable.h"
#include "Components/BoxComponent.h"
#include "FleshCube.generated.h"


struct ConnectedCubeInfo
{
public:
	AFleshCube* ConnectedCube;
	UFleshCubeSideBase* ConnectedFace;

public:
	ConnectedCubeInfo(AFleshCube* ConnectedCube, UFleshCubeSideBase* ConnectedFace) : ConnectedCube(ConnectedCube), ConnectedFace(ConnectedFace) {}
};

UCLASS()
class HELLGAME_API AFleshCube : public AInteractableBase
{
	GENERATED_BODY()

private:
	ESideType PreviousLeftSide;
	ESideType PreviousFrontSide;
	ESideType PreviousRightSide;
	ESideType PreviousBackSide;
	UDataTable* FaceBlueprintTable;
	bool bStartSidesGenerated = false;
	bool bCurrentlyCarried = false;
	bool bCanSendStartSignal = false;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blueprint Faces")
	TMap<ESideType, TSubclassOf<UFleshCubeSideBase>> BlueprintFaces;

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
	UStaticMeshComponent* LeftSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* RightSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* FrontSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* BackSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* TopSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* BottomSideMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Side Colliders")
	UBoxComponent* LeftSideBoxCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Side Colliders")
	UBoxComponent* FrontSideBoxCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Side Colliders")
	UBoxComponent* RightSideBoxCollider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Side Colliders")
	UBoxComponent* BackSideBoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* LeftSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* FrontSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* RightSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* BackSide;

	ConnectedCubeInfo LeftConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
	ConnectedCubeInfo FrontConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
	ConnectedCubeInfo RightConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
	ConnectedCubeInfo BackConnectedCube = ConnectedCubeInfo(nullptr, nullptr);

	UFUNCTION()
	void OnSideCollisionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSideCollisionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void SendActivationSignal(UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType);


private:
	void SetupBaseMesh();
	void SetupSideMeshes();
	void SetupSides();
	void SetupStartSides();
	void SetupSide(UStaticMeshComponent* SideMeshComponent, ESideType SideType, ESideType& PreviousType, UFleshCubeSideBase* CubeSide);
	void TemporaryReferenceFiller(ESideType SideType, const TCHAR* Reference);
	bool HasConnectedNeighbour();

protected:
	virtual void BeginPlay() override;

	virtual void OnPickUp_Implementation(AActor* Caller) override;
	virtual void OnDropPickUp_Implementation(AActor* Caller) override;

public:
	AFleshCube();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFleshCubeSideBase* GetCubeSideByCollider(FString ColliderName);
};
