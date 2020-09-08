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
	//Cast<UPrimitiveComponent>(this->GetRootComponent())->SetSimulatePhysics(false);
	FBodyInstance* Componenet = Cast<UPrimitiveComponent>(this->GetRootComponent())->GetBodyInstance();
	Componenet->bLockXTranslation = true;
	Componenet->bLockYTranslation = true;
	//Componenet->bLockZTranslation = true;
	Componenet->bLockXRotation = true;
	Componenet->bLockYRotation = true;
	Componenet->bLockZRotation = true;
	Componenet->SetDOFLock(EDOFMode::SixDOF);
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::OnInteract_Implementation(AActor* Caller, FVector ImpactPoint)
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
			//Cast<UPrimitiveComponent>(this->GetRootComponent())->SetSimulatePhysics(true);
			FBodyInstance* Componenet = Cast<UPrimitiveComponent>(this->GetRootComponent())->GetBodyInstance();
			Componenet->bLockXTranslation = false;
			Componenet->bLockYTranslation = false;
			//Componenet->bLockZTranslation = false;
			Componenet->bLockXRotation = false;
			Componenet->bLockYRotation = false;
			Componenet->bLockZRotation = false;
			Componenet->SetDOFLock(EDOFMode::SixDOF);
			IPickupAble::Execute_OnPickUp(Actor, Caller, ImpactPoint);
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

void AInteractableBase::OnPickUp_Implementation(AActor* Caller, FVector ImpactPoint)
{
	if (bCanPickup)
	{
		if (Interactor->PickupComponent)
		{
			Interactor->PickupComponent->PickUp(this, ImpactPoint);
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

