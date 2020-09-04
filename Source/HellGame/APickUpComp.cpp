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

	float hello = AngelsBetween2DVectors(HoldItem->K2_GetComponentLocation(), HoldPosition->K2_GetComponentLocation(), HoldItem->GetForwardVector());

	HoldPosition->GetForwardVector().Normalize();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("APickUpComp: " + FString::SanitizeFloat(hello)));

	HoldItem->SetWorldRotation(HoldPosition->GetComponentRotation());
	CurrentForward = HoldPosition->GetComponentQuat();
	PhysicsHandle->GrabComponentAtLocationWithRotation(HoldItem, NAME_None, HoldItem->GetComponentLocation(), HoldItem->GetComponentRotation());
	//PhysicsHandle->SetAngularStiffness(10000);

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
	FRotator testRot = HoldPosition->K2_GetComponentRotation();
	UE_LOG(LogTemp, Warning, TEXT("%f . %f . %f"), testRot.Yaw, testRot.Roll, testRot.Pitch);
	//testRot.Pitch = 0;
	FQuat TestQuat = testRot.Quaternion();

	PhysicsHandle->SetTargetLocationAndRotation(HoldPosition->GetComponentLocation(), testRot);
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

