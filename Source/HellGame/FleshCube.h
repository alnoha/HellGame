#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FleshCubeSideBase.h"
#include "SideTypes.h"
#include "InteractableBase.h"
#include "CubeFaceData.h"
#include "EyeComponentData.h"
#include "Components/BoxComponent.h"
#include "FleshCube.generated.h"

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cube Basic Events")
	void HitGround();
	virtual void HitGround_Implementation();

	UPROPERTY(EditAnywhere)
	UCubeFaceData* FaceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Respawn", meta = (ToolTip = "Where in world location the cube should respawn"))
	AActor* CubeRespawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides", meta = (ToopTip = "Which cube face should the left side of the cube have."))
	ESideType LeftSideType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides", meta = (ToolTip = "Which cube face should the front side of the cube have."))
	ESideType FrontSideType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides", meta = (ToolTip = "Which cube face should the right side of the cube have."))
	ESideType RightSideType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cube Sides", meta = (ToolTip = "Which cube face should the back side of the cube have."))
	ESideType BackSideType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (ToolTip = "Cubes base mesh (The inside of the cube)."))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes", meta = (ToolTip = "Skeletal Mesh Component for the left side."))
	USkeletalMeshComponent* LeftSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes", meta = (ToolTip = "Skeletal Mesh Component for the front side."))
	USkeletalMeshComponent* RightSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes", meta = (ToolTip = "Skeletal Mesh Component for the right side."))
	USkeletalMeshComponent* FrontSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes", meta = (ToolTip = "Skeletal Mesh Component for the back side."))
	USkeletalMeshComponent* BackSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes", meta = (ToolTip = "Skeletal Mesh Component for the top side."))
	USkeletalMeshComponent* TopSideMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SideMeshes", meta = (ToolTip = "Skeletal Mesh Component for the bottom side."))
	USkeletalMeshComponent* BottomSideMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stationary Mesh", meta = (ToolTip = "The mesh to use for bottom mesh when a cube is stationary."))
	USkeletalMesh* BoltedMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stationary Mesh", meta = (ToolTip = "The material to use for bottom mesh when a a cube is stationary."))
	UMaterialInstance* BoltedMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube Side Components", meta = (ToolTip = "Current fleshcubeside at the left side of the cube."))
	UFleshCubeSideBase* LeftSide;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube Side Components", meta = (ToolTip = "Current fleshcubeside at the front side of the cube."))
	UFleshCubeSideBase* FrontSide;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube Side Components", meta = (ToolTip = "Current fleshcubeside at the right side of the cube."))
	UFleshCubeSideBase* RightSide;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Cube Side Components", meta = (ToolTip = "Current fleshcubeside at the back side of the cube."))
	UFleshCubeSideBase* BackSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snot", meta = (ToolTip = "The snotscale for the left side, will only affect the left side if there's a nose face there."))
	FVector SnotScaleLeftSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snot", meta = (ToolTip = "The snotscale for the front side, will only affect the front side if there's a nose face there."))
	FVector SnotScaleFrontSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snot", meta = (ToolTip = "The snotscale for the right side, will only affect the right side if there's a nose face there."))
	FVector SnotScaleRightSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snot", meta = (ToolTip = "The snotscale for the back side, will only affect the back side if there's a nose face there."))
	FVector SnotScaleBackSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye", meta = (ToolTip = "Eyedata for the left side, will only affect the left side if there's a eye face there."))
	FEyeComponentData EyeDataLeftSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye", meta = (ToolTip = "Eyedata for the front side, will only affect the front side if there's a eye face there."))
	FEyeComponentData EyeDataFrontSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye", meta = (ToolTip = "Eyedata for the right side, will only affect the right side if there's a eye face there."))
	FEyeComponentData EyeDataRightSide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye", meta = (ToolTip = "Eyedata for the back side, will only affect the back side if there's a eye face there."))
	FEyeComponentData EyeDataBackSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace", meta = (ToolTip = "How far away from the cube it will search for connected cubes."))
	float CubeSideTraceDistance = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Trace", meta = (ToolTip = "The distance between the cube's 0 vector and the ground to have the cube see itself as grounded."))
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
	void SetupSide(USkeletalMeshComponent*& SideMeshComponent, ESideType& SideType, ESideType& PreviousType, UFleshCubeSideBase*& CubeSide, FEyeComponentData EyeComponentData, FVector SnotScale);
	void LatchCube(USkeletalMeshComponent* Start, UPrimitiveComponent* CubeSide);
	bool bHasLatched;

protected:
	virtual void BeginPlay() override;

	virtual void OnPickUp_Implementation(AActor* Caller, FVector ImpactComponent ) override;
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