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
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::OnInteract_Implementation(AActor* Caller)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, Caller->GetName() + TEXT(" Called AInteractableBase::OnInteract_Implementation"));
	if (this->Implements<UPickupAble>())
	{
		//IPickupAble::OnPickUp();
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("Implements PickupInterface"));
	}
}

void AInteractableBase::OnBeginFocus_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("AInteractableBase::OnBeginFocus_Implementation"));
	if (!bIsFocused)
	{
		HUD->ShowWidget(PromptWidget);
		bIsFocused = true;
	}
}

void AInteractableBase::OnEndFocus_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("AInteractableBase::OnEndFocus_Implementation"));
	if (bIsFocused)
	{
		HUD->HideWidget();
		bIsFocused = false;
	}
}

