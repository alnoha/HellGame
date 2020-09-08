// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCubeSideBase.h"

UFleshCubeSideBase::UFleshCubeSideBase()
{
}

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

void UFleshCubeSideBase::ReceivedActivationSignal_Implementation(UFleshCubeSideBase* SendingSide, ESideType SendingType, FTransform SideTransform)
{
}

void UFleshCubeSideBase::ReceivedStopSignal_Implementation()
{
}

void UFleshCubeSideBase::ReceiveRemoteActivationSignal_Implementation(FTransform SideTransform)
{
}

void UFleshCubeSideBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFleshCubeSideBase::SetupMesh(USkeletalMeshComponent* parent)
{
	if (BaseMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseMEsh nullptr"));
	}
	if (FaceMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FaceMesh nullptr"));
	}
	if (FaceMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("parent nullptr"));
	}
	if (BaseMesh != nullptr && FaceMesh != nullptr && parent != nullptr)
	{
		BaseMesh->SetupAttachment(parent);
		BaseMesh->SetSkeletalMesh(FaceMesh);
	}
}

USkeletalMesh* UFleshCubeSideBase::GetFaceMesh()
{
	return FaceMesh;
}

void UFleshCubeSideBase::SetCurrentSideType(ESideType NewSideType)
{
	CurrentSideType = NewSideType;
}

ESideType UFleshCubeSideBase::GetCurrentSideType()
{
	return CurrentSideType;
}
