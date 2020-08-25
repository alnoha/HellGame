// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeSideComponent.h"

UCubeSideComponent::UCubeSideComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	UE_LOG(LogTemp, Warning, TEXT("Creating cube side component"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	
	USceneComponent* rootComponent = nullptr;
	AActor* owner = GetOwner();

	if (owner != nullptr)
	{
		rootComponent = owner->GetRootComponent();
	}

	if (rootComponent != nullptr)
	{
		BaseMesh->SetupAttachment(rootComponent);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
}

void UCubeSideComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCubeSideComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCubeSideComponent::InitComponent(ECubeSide sideType)
{
	m_CubeType = sideType;
}

