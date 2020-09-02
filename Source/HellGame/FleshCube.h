// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FleshCubeSideBase.h"
#include "SideTypes.h"
#include "InteractableBase.h"
#include "Engine/DataTable.h"
#include "FleshCube.generated.h"

UCLASS()
class HELLGAME_API AFleshCube : public AInteractableBase
{
	GENERATED_BODY()

private:
	UDataTable* FaceBlueprintTable;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blueprint Faces")
	TMap<ESideType, TSubclassOf<UFleshCubeSideBase>> BlueprintFaces;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* LeftSideMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* RightSideMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* FrontSideMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* BackSideMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* TopSideMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideMeshes")
	UStaticMeshComponent* BottomSideMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* LeftSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* FrontSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* RightSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Side Components")
	UFleshCubeSideBase* BackSide;

private:
	ESideType PreviousLeftSide;
	ESideType PreviousFrontSide;
	ESideType PreviousRightSide;
	ESideType PreviousBackSide;

private:
	void SetupBaseMesh();
	void SetupSideMeshes();
	void SetupSides();
	void SetupTopBottomSides();
	void SetupSide(UStaticMeshComponent* SideMeshComponent, ESideType SideType, ESideType& PreviousType, UFleshCubeSideBase* CubeSide);
	void TemporaryReferenceFiller(ESideType SideType, const TCHAR* Reference);

protected:
	virtual void BeginPlay() override;

public:
	AFleshCube();

	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
