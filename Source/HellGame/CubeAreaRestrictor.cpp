// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAreaRestrictor.h"

// Sets default values
ACubeAreaRestrictor::ACubeAreaRestrictor()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACubeAreaRestrictor::OnBeginOverlap);

}

void ACubeAreaRestrictor::OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AFleshCube>())
	{
		AFleshCube* Cube = Cast<AFleshCube>(OtherActor);
		IPickupAble::Execute_OnDropPickUp(Cube, this);
		if (Cube->CubeRespawnPoint != nullptr)
		{
			OtherComp->SetPhysicsAngularVelocity(FVector(0.0f));
			OtherComp->SetPhysicsLinearVelocity(FVector(0.0f));
			Cube->SetActorRotation(Cube->CubeRespawnPoint->GetActorRotation());
			Cube->SetActorLocation(Cube->CubeRespawnPoint->GetActorLocation());
		}
	}
}



