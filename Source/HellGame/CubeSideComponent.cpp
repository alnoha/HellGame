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

void UCubeSideComponent::InitComponent(ECubeSide sideType, FVector position, FQuat rotation)
{
	m_CubeType = sideType;

	UE_LOG(LogTemp, Warning, TEXT("Creating Eye Component %d"), position.X);

	BaseMesh->SetRelativeLocation(position);
	BaseMesh->SetRelativeRotation(rotation);
	BaseMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.0f));
}

