// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCubeSideBase.h"

UFleshCubeSideBase::UFleshCubeSideBase()
{
	PrimaryComponentTick.bCanEverTick = false;

	UE_LOG(LogTemp, Warning, TEXT("Getting spawned! :D"));

	SetupBaseMesh();
}

void UFleshCubeSideBase::SetupBaseMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("Mesh setup has started"));
	// Create base cube mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test"));
	BaseMesh->bEditableWhenInherited = true;

	UE_LOG(LogTemp, Warning, TEXT("Mesh setup finished"));
}

void UFleshCubeSideBase::BeginPlay()
{
	Super::BeginPlay();
}

void UFleshCubeSideBase::TextPrinter_Implementation()
{
}

void UFleshCubeSideBase::ReceivedStopSignal_Implementation()
{
}

void UFleshCubeSideBase::ReceiveRemoteActivationSignal_Implementation()
{
}

void UFleshCubeSideBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFleshCubeSideBase::SetupMesh(UStaticMeshComponent* parent)
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
		BaseMesh->SetStaticMesh(FaceMesh);
	}
}

UStaticMesh* UFleshCubeSideBase::GetFaceMesh()
{
	return FaceMesh;
}
