// Fill out your copyright notice in the Description page of Project Settings.


#include "APickUpComp.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

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

void UAPickUpComp::PickUp(AActor* Actor, FVector HitLocation)
{
	HoldActor = Actor;
	UActorComponent* whatever = HoldActor->GetComponentByClass(UStaticMeshComponent::StaticClass());

	temp = (UStaticMeshComponent*)whatever;
	ItemMesh = temp->GetStaticMesh();
	HoldItem = (UPrimitiveComponent*)temp;

	float theta =FMath::RadiansToDegrees(FMath::Atan2(HitLocation.Y - HoldActor->GetActorLocation().Y, HitLocation.X - HoldActor->GetActorLocation().X));
	theta -= HoldActor->GetActorRotation().Yaw;
	if (theta < 0)
	{
		theta += 360;
	}
	int degreestoadd;
	int newfloat = int(theta)% 90;
	if (newfloat >= 45)
	{
		degreestoadd = (90 -newfloat) + theta;
	
	}
	else
	{
		degreestoadd = (newfloat * -1) + theta;	
	}

	if (degreestoadd  >= 135 && degreestoadd  <=  225 || degreestoadd <= 45 || degreestoadd >= 315)
	{
		degreestoadd += 180;
	}
	FRotator newrotation = { 0,static_cast<float>(degreestoadd),0 };
	HoldPosition->SetRelativeRotation(newrotation.Quaternion());
	PhysicsHandle->GrabComponentAtLocationWithRotation(HoldItem, NAME_None, HoldItem->GetComponentLocation(), HoldItem->GetComponentRotation());
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


void UAPickUpComp::RotateSetup(FRotator DeltaRotation)
{
	oldRotationTest = HoldPosition->GetRelativeRotation().Quaternion();
	rotationTest = HoldPosition->GetRelativeRotation().Quaternion() * DeltaRotation.Quaternion();
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
		RotateSetup({ 0,0 + RotateAmount,0 });
	}

}

void UAPickUpComp::RotateRight()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup({ 0,0 - RotateAmount,0 });
	}
}

void UAPickUpComp::RotateUp()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup({ 0 - RotateAmount, 0,  0 });
	}
}

void UAPickUpComp::RotateDown()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		RotateSetup({ 0 + RotateAmount, 0, 0 });
	}
}

void UAPickUpComp::AltRotateLeft()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		FRotator TempDeltaRotation = { 0,0 + AltRotateAmount,0 };
		HoldPosition->AddRelativeRotation(TempDeltaRotation.Quaternion());
		CurrentForward.AngularDistance(oldRotationTest);

	}
}

void UAPickUpComp::AltRotateRight()
{
	if (HoldItem != nullptr && RotatingObject == false)
	{
		FRotator TempDeltaRotation = { 0,0 - AltRotateAmount,0 };
		HoldPosition->AddRelativeRotation(TempDeltaRotation.Quaternion());
	}
}

void UAPickUpComp::Rotate(float deltaTime)
{

	//HoldPosition->SetRelativeRotation(FMath::Lerp(OldRotation, NewRotation, LerpTimer));
	HoldPosition->SetRelativeRotation(FMath::Lerp(oldRotationTest, rotationTest, LerpTimer));

	LerpTimer += deltaTime * LerpSpeed;

	if (LerpTimer >= 0.95f)
	{
		LerpTimer = 0;
		HoldPosition->SetRelativeRotation(rotationTest);
		RotatingObject = false;

		// temp->GetBodyInstance()->bLockXRotation = true;
		// temp->GetBodyInstance()->bLockYRotation = true;
		// temp->GetBodyInstance()->bLockZRotation = false;
	}

}

void UAPickUpComp::ResetHoldingPoint()
{
	HoldPosition->SetRelativeRotation(startRotation);
	HoldPosition->SetRelativeLocation(startPosition);
}

void UAPickUpComp::UpdateHoldItemPosition()
{

	PhysicsHandle->SetTargetLocationAndRotation(HoldPosition->GetComponentLocation(), HoldPosition->GetComponentRotation());
	//HoldActor->SetActorRotation(HoldItem->GetRelativeRotation());

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
		NewRotation = startRotation = HoldPosition->GetRelativeRotation();

	}
}

void UAPickUpComp::WriteErrorMessage(FString message)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("APickUpComp: " + message));
	UE_LOG(LogTemp, Error, TEXT("APickUpComp: %s"), *message);
}






void UAPickUpComp::ConvertRotation()
{

}

float UAPickUpComp::DotProd(FVector vec1, FVector vec2)
{
	vec1.Normalize();
	vec2.Normalize();

	return FVector::DotProduct(vec1, vec2);
}

FQuat UAPickUpComp::QuatDifferens(FQuat Quat1, FQuat Quat2)
{
	return Quat1.Inverse() * Quat2;
}

float UAPickUpComp::AngelsBetween2DVectors(FVector Vector1, FVector Vector2, FVector ForwardVector)
{
	FVector2D ConvertedVector1 = { Vector1.X,Vector1.Y };
	FVector2D ConvertedVector2 = { Vector2.X,Vector2.Y };
	FVector2D ConvertedVector3 = { ForwardVector.X,ForwardVector.Y };
	FVector2D tempthingy = ConvertedVector1 - ConvertedVector2;
	tempthingy.Normalize();
	ConvertedVector3.Normalize();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("APickUpComp: " + FString::SanitizeFloat(FVector2D::DotProduct(ConvertedVector1 , ConvertedVector2) / (ConvertedVector1.Size() * ConvertedVector2.Size()))));
	return UKismetMathLibrary::DegAcos(FVector2D::DotProduct(ConvertedVector3, tempthingy));
}

