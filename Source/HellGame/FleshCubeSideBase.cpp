// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCubeSideBase.h"

UFleshCubeSideBase::UFleshCubeSideBase()
{
}

#pragma region Unreal Methods
void UFleshCubeSideBase::SetupBaseMesh()
{
	// Create base cube mesh
	BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Test"));
	BaseMesh->bEditableWhenInherited = true;
}

void UFleshCubeSideBase::BeginPlay()
{
	Super::BeginPlay();
}

void UFleshCubeSideBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma endregion

#pragma region Event Implementations
void UFleshCubeSideBase::ReceivedActivationSignal_Implementation(UFleshCubeSideBase* SendingSide, ESideType SendingType, USkeletalMeshComponent* MeshComponent, FTransform SideTransform)
{
}

void UFleshCubeSideBase::ReceivedStopSignal_Implementation()
{
}

void UFleshCubeSideBase::ReceiveRemoteActivationSignal_Implementation(FTransform SideTransform)
{
}

void UFleshCubeSideBase::Initialize_Side_Implementation(FEyeComponentData EyeData, FVector SnotScale)
{
}

#pragma endregion

#pragma region Getters
USkeletalMesh* UFleshCubeSideBase::GetFaceMesh()
{
	return FaceMesh;
}

UMaterialInstance* UFleshCubeSideBase::GetFaceMaterial()
{
	return FaceMaterial;
}

TSubclassOf<UAnimInstance> UFleshCubeSideBase::GetAnimationInstance()
{
	return FaceAnimation;
}

ESideType UFleshCubeSideBase::GetCurrentSideType()
{
	return CurrentSideType;
}
#pragma endregion

#pragma region Setters
void UFleshCubeSideBase::SetCurrentSideType(ESideType NewSideType)
{
	CurrentSideType = NewSideType;
}
#pragma endregion