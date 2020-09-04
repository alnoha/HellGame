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

	//TSubclassOf<UCubeFaceData> FaceData;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace")
		float CubeSideTraceDistance = 200.0f;

	/*How far the cube will snap to the other cube. (Default = 250)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace")
		float CubeSnapDistance = 250.0f;

	UFUNCTION()
		void OnSideCollisionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnSideCollisionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void SendActivationSignal(AFleshCube* SendingCube, UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType, bool ReturnSignal = false);


private:
	void ReadFaceData();
	void SetupBaseMesh();
	void SetupSideMeshes();
	void SetupSides();
	void SetupStartSides();
	void SetupSide(UStaticMeshComponent*& SideMeshComponent, ESideType& SideType, ESideType& PreviousType, UFleshCubeSideBase*& CubeSide);
	void SetupLeftSide();
	void SetupFrontSide();
	void SetupRightSide();
	void SetupBackSide();
	void LatchCube(FVector Start, UPrimitiveComponent* CubeSide);

protected:
	virtual void BeginPlay() override;

	virtual void OnPickUp_Implementation(AActor* Caller) override;
	virtual void OnDropPickUp_Implementation(AActor* Caller) override;

public:
	AFleshCube();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Cube sides")
		UFleshCubeSideBase* GetCubeSideByCollider(FString Collider);

	UFleshCubeSideBase* GetCubeSideByMesh(FString MeshName);
};