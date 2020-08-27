// Fill out your copyright notice in the Description page of Project Settings.


#include "APickUpComp.h"

// Sets default values for this component's properties
UAPickUpComp::UAPickUpComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAPickUpComp::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	startPosition = HoldPosition->GetRelativeLocation();
	startRotation = HoldPosition->GetRelativeRotation();
	this->SetComponentTickEnabled(false);	
	
}


// Called every frame
void UAPickUpComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateHoldItemPosition();
	if (RotatingObject)
	{
		Rotate(DeltaTime);
	}
}

void UAPickUpComp::PickUp(UPrimitiveComponent* actor)
{	
	HoldItem = actor;
	HoldItem->SetWorldRotation(HoldPosition->GetComponentRotation());
	test->GrabComponentAtLocationWithRotation(HoldItem, NAME_None, HoldItem->GetComponentLocation(), HoldItem->GetComponentRotation());
	this->SetComponentTickEnabled(true);
}

void UAPickUpComp::Drop()
{
	test->ReleaseComponent();
	HoldItem = nullptr;
	ResetHoldingPoint();
	this->SetComponentTickEnabled(false);
}

void UAPickUpComp::RotateSetup()
{
	NewRotation = OldRotation = HoldPosition->GetRelativeRotation();
	RotatingObject = true;
}

FRotator UAPickUpComp::AltRotateSetup()
{
	
	return HoldPosition->GetRelativeRotation();
}

void UAPickUpComp::RotateLeft()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup();
		NewRotation.Yaw += RotateAmount;
	}

}

void UAPickUpComp::RotateRight()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup();
		NewRotation.Yaw -= RotateAmount;
	}
}

void UAPickUpComp::RotateUp()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup();
		NewRotation.Pitch += RotateAmount;
	}
}

void UAPickUpComp::RotateDown()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup();
		NewRotation.Pitch -= RotateAmount;
	}
}

void UAPickUpComp::AltRotateLeft()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		FRotator TempRotation = HoldPosition->GetRelativeRotation();
		TempRotation.Yaw += AltRotateAmount;		
		HoldPosition->SetRelativeRotation(TempRotation);
	}
}

void UAPickUpComp::AltRotateRight()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		FRotator TempRotation = HoldPosition->GetRelativeRotation();
		TempRotation.Yaw -= AltRotateAmount;
		HoldPosition->SetRelativeRotation(TempRotation);
	}
}

void UAPickUpComp::Rotate(float deltaTime)
{	
	HoldPosition->SetRelativeRotation(FMath::Lerp(OldRotation, NewRotation, lerpTimer));
	lerpTimer += deltaTime * lerpSpeed;

	if (lerpTimer >= 0.95f)
	{
	 lerpTimer = 0;
	 HoldPosition->SetRelativeRotation(NewRotation);
	 RotatingObject = false;
	}
	
}

void UAPickUpComp::ResetHoldingPoint()
{
	HoldPosition->SetRelativeRotation(startRotation);
	HoldPosition->SetRelativeLocation(startPosition);
}

void UAPickUpComp::UpdateHoldItemPosition()
{
	
		test->SetTargetLocationAndRotation(HoldPosition->GetComponentLocation(),HoldPosition->GetComponentRotation());
		
}




