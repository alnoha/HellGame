// Copyright Epic Games, Inc. All Rights Reserved.

#include "HellGameCharacter.h"
#include "HellGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Interactable.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AHellGameCharacter

AHellGameCharacter::AHellGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

}

void AHellGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PickupComponent = Cast<AActor>(this)->FindComponentByClass<UAPickUpComp>();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHellGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AHellGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHellGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHellGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHellGameCharacter::LookUpAtRate);
}

void AHellGameCharacter::InteractTrace()
{
	FVector Pos;
	FRotator Rot;
	GetController()->GetPlayerViewPoint(Pos, Rot);

	FCollisionQueryParams TraceParams;
	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Pos, Pos + (Rot.Vector() * InteractDistance), ECC_Visibility, TraceParams);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		//if (bIsDrawingPrompt == false)
		//{
		//	HidePrompt();
		//	Interactable = nullptr;
		//}
		if (HitActor->Implements<UInteractable>())
		{
			if (HitActor != Interactable)
			{
				HidePrompt();
			}
			Interactable = HitActor;
			ImpactPoint = { Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z };
			IInteractable::Execute_OnBeginFocus(Interactable);
			bIsDrawingPrompt = true;
		}
		else
		{
			HidePrompt();
			Interactable = nullptr;
		}
	}
	else
	{
		HidePrompt();
		Interactable = nullptr;
	}
}

void AHellGameCharacter::HidePrompt()
{
	if (Interactable)
	{
		if (Interactable->Implements<UInteractable>())
		{
			IInteractable::Execute_OnEndFocus(Interactable);
			bIsDrawingPrompt = false;
		}
	}
}

void AHellGameCharacter::OnInteract_Implementation(AActor* Actor)
{
	// Check if we have something to interact whit, else just return.
	if (Actor == nullptr)
	{
		if (Interactable)
		{
			Actor = Interactable;
		}
		else
		{
			return;
		}
	}

	//Failsafe if an actor from a blueprint doesn't implements the interacteable interface
	if (Actor->Implements<UInteractable>())
	{
		IInteractable::Execute_OnInteract(Actor, this, ImpactPoint);
	}

}

void AHellGameCharacter::Tick(float DeltaTime)
{
	if (bIsHoldingObject == false)
	{
		InteractTrace();
	}
}

void AHellGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AHellGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AHellGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHellGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
