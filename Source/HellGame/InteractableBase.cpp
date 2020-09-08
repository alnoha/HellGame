// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	HUD = Cast<AHellGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	Cast<UPrimitiveComponent>(this->GetRootComponent())->SetSimulatePhysics(false);
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::OnInteract_Implementation(AActor* Caller)
{
	AActor* Actor = Cast<AActor>(this);
	if (Actor->Implements<UPickupAble>())
	{
		Interactor = Cast<AHellGameCharacter>(Caller);
		if (Interactor->bIsHoldingObject)
		{
			IPickupAble::Execute_OnDropPickUp(Actor, Caller);
		}
		else
		{
			Cast<UPrimitiveComponent>(this->GetRootComponent())->SetSimulatePhysics(true);
			IPickupAble::Execute_OnPickUp(Actor, Caller);
		}
	}
}
void AInteractableBase::OnBeginFocus_Implementation()
{
	if (!bIsFocused)
	{
		if (bCanPickup)
		{
			HUD->UpdateCrosshair(HUD->CrosshairTextureMap[ECrosshairTypes::PICKUP]);
		}
		HUD->ShowWidget(PromptWidget);
		bIsFocused = true;
	}
}

void AInteractableBase::OnEndFocus_Implementation()
{
	if (bIsFocused)
	{
		HUD->UpdateCrosshair(HUD->CrosshairTextureMap[ECrosshairTypes::DEFAULT]);
		HUD->HideWidget();
		bIsFocused = false;
	}
}

void AInteractableBase::OnPickUp_Implementation(AActor* Caller)
{
	if (bCanPickup)
	{
		if (Interactor->PickupComponent)
		{
			Interactor->PickupComponent->PickUp(this);
			Interactor->HidePrompt();
			Interactor->bIsHoldingObject = true;
			HUD->UpdateCrosshair(HUD->CrosshairTextureMap[ECrosshairTypes::ROTATE]);
		}
	}
}

void AInteractableBase::OnDropPickUp_Implementation(AActor* Caller)
{
	if (bCanPickup)
	{
		if (Interactor->PickupComponent)
		{
			Interactor->PickupComponent->Drop();
			Interactor->bIsHoldingObject = false;
			HUD->UpdateCrosshair(HUD->CrosshairTextureMap[ECrosshairTypes::DEFAULT]);
		}
	}
}

