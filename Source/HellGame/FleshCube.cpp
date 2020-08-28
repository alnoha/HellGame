// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCube.h"
#include "CubeSideComponent.h"
#include "EyeSideComponent.h"

// Sets default values
AFleshCube::AFleshCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));

	leftSideComponent = CreateDefaultSubobject<UCubeSideComponent>(TEXT("Left side component"));
	leftSideComponent->InitComponent(leftSide, FVector(-50,-50,50), FQuat(90,0,0,0));

	CreateEyeComponentQueue();
}

UCubeSideComponent* AFleshCube::PopulateSideSlot(ECubeSide cubeSide)
{
	UCubeSideComponent* sideComponent = nullptr;

	switch (cubeSide)
	{
	case ECubeSide::Eye:
		UE_LOG(LogTemp, Warning, TEXT("Creating Eye Component"));

		/*if (eyeComponents.Dequeue(sideComponent))
		{
			this->AddOwnedComponent(sideComponent);
		}*/
	}

	return sideComponent;
}

void AFleshCube::CreateEyeComponentQueue()
{
}

// Called when the game starts or when spawned
void AFleshCube::BeginPlay()
{
	Super::BeginPlay();

	leftSideComponent = PopulateSideSlot(leftSide);
	rightSideComponent = PopulateSideSlot(rightSide);
	frontSideComponent = PopulateSideSlot(frontSide);
	backSideComponent = PopulateSideSlot(backSide);
	topSideComponent = PopulateSideSlot(topSide);
	bottomSideComponent = PopulateSideSlot(bottomSide);
}

// Called every frame
void AFleshCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

