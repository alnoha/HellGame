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
	StartCheck();
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

void UAPickUpComp::PickUp(AActor* actor)
{	
	
	HoldActor = actor;
	UActorComponent* whatever = HoldActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
	temp = (UStaticMeshComponent*)whatever;
	ItemMesh = temp->GetStaticMesh();
	HoldItem = (UPrimitiveComponent*)temp;
	
	HoldItem->SetWorldRotation(HoldPosition->GetComponentRotation());	
	PhysicsHandle->GrabComponentAtLocationWithRotation(HoldItem, NAME_None, HoldItem->GetComponentLocation(), HoldItem->GetComponentRotation());	
	PhysicsHandle->SetAngularStiffness(10000);
	IsHolding = true;
	this->SetComponentTickEnabled(true);
}

void UAPickUpComp::Drop()
{
	PhysicsHandle->ReleaseComponent();
	HoldItem = nullptr;
	ResetHoldingPoint();
	IsHolding = false;
	this->SetComponentTickEnabled(false);
}

void UAPickUpComp::RotateSetup()
{
	NewRotation = OldRotation = HoldPosition->GetComponentRotation();
	RotatingObject = true;
}

FRotator UAPickUpComp::AltRotateSetup()
{
	
	return HoldPosition->GetRelativeRotation();
}

void UAPickUpComp::MoveForward()
{
	if (HoldPosition->GetRelativeLocation().X < MoveMaxLimit)
	{
		FVector NewVec = HoldPosition->GetRelativeLocation();
		if (HoldPosition->GetRelativeLocation().X + MoveAmount > MoveMaxLimit)
		{		
			NewVec.X = MoveMaxLimit;
			HoldPosition->SetRelativeLocation(NewVec);
		}
		else
		{
			NewVec.X += MoveAmount;
			HoldPosition->SetRelativeLocation(NewVec);
		}
	}

}

void UAPickUpComp::MoveBack()
{
	if (HoldPosition->GetRelativeLocation().X > MoveMinLimit)
	{
		FVector NewVec = HoldPosition->GetRelativeLocation();
		if (HoldPosition->GetRelativeLocation().X - MoveAmount < MoveMinLimit)
		{			
			NewVec.X = MoveMinLimit;
			HoldPosition->SetRelativeLocation(NewVec);
		}
		else
		{			
			NewVec.X -= MoveAmount;
			HoldPosition->SetRelativeLocation(NewVec);
		}
		
	}
}

void UAPickUpComp::RotateLeft()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup();
		/*temp->GetBodyInstance()->bLockXRotation = true;
		temp->GetBodyInstance()->bLockYRotation = true;
		temp->GetBodyInstance()->bLockZRotation = false;*/
		
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
		temp->BodyInstance.bLockRotation = true;
		temp->BodyInstance.bLockYRotation = false;
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
	HoldPosition->SetWorldRotation(FMath::Lerp(OldRotation, NewRotation, LerpTimer));
	LerpTimer += deltaTime * LerpSpeed;

	if (LerpTimer >= 0.95f)
	{
	 LerpTimer = 0;
	 HoldPosition->SetWorldRotation(NewRotation);
	 RotatingObject = false;

	 temp->GetBodyInstance()->bLockXRotation = true;
	 temp->GetBodyInstance()->bLockYRotation = true;
	 temp->GetBodyInstance()->bLockZRotation = false;
	}
	
}

void UAPickUpComp::ResetHoldingPoint()
{
	HoldPosition->SetRelativeRotation(startRotation);
	HoldPosition->SetRelativeLocation(startPosition);
}

void UAPickUpComp::UpdateHoldItemPosition()
{	
	PhysicsHandle->SetTargetLocationAndRotation(HoldPosition->GetComponentLocation(),HoldPosition->GetComponentRotation());		
}

void UAPickUpComp::StartCheck()
{
	bool ForceShutDown = false;
	if (HoldPosition == nullptr)
	{
		WriteErrorMessage(" HoldPosition Not Connected");
		ForceShutDown = true;
	}
	if (PhysicsHandle == nullptr)
	{
		WriteErrorMessage(" PhysicsHandle Not Connected");
		ForceShutDown = true;
	}
	if (ForceShutDown)
	{
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
	else
	{
		Owner = GetOwner();
		startPosition = HoldPosition->GetRelativeLocation();
		startRotation = HoldPosition->GetRelativeRotation();
	}
}

void UAPickUpComp::WriteErrorMessage(FString message)
{
	
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("APickUpComp: " + message));
	UE_LOG(LogTemp, Error, TEXT("APickUpComp: %s"), *message);
}


