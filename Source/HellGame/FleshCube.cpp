// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCube.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFleshCube::AFleshCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupBaseMesh();
	SetupSideMeshes();
}

void AFleshCube::SendActivationSignal(AFleshCube* SendingCube, UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType, bool ReturnSignal)
{
	/*GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Activation signal"));
	if (!FaceData->SideData.Contains(ReceivingSide->GetCurrentSideType()))
	{
		const UEnum* SideTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESideType"));
		UE_LOG(LogTemp, Warning, TEXT("Can't find receiving side in data asset %s"), *SideTypeEnum->GetEnumName((int32)ReceivingSide->GetCurrentSideType()));
		return;
	}

	if (FaceData->SideData[ReceivingSide->GetCurrentSideType()].FaceMatches.Contains(SendingType))
	{
	*/
	bool bFoundaSide = false;

	if (ReceivingSide != LeftSide)
	{
		LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (ReceivingSide != FrontSide)
	{
		FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (ReceivingSide != RightSide)
	{
		RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (ReceivingSide != BackSide)
	{
		BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (!bFoundaSide)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Could not find a side"));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, ReceivingSide->GetName());
	}
	/*}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Face match does not contain type"));
	}*/
}

void AFleshCube::SetupBaseMesh()
{
	// Create base cube mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxBase"));
	this->SetRootComponent(BaseMesh);

	// Setup Physics
	BaseMesh->SetSimulatePhysics(true);
}

void AFleshCube::SetupSideMeshes()
{
	SetupSideMesh(LeftSideMeshComponent, BaseMesh, FVector(0.0), FRotator(0.0f, 270.f, 0.0f), FName("Left Side Mesh"));
	SetupSideMesh(FrontSideMeshComponent, BaseMesh, FVector(0.0), FRotator(0.0f), FName("Front Side Mesh"));
	SetupSideMesh(RightSideMeshComponent, BaseMesh, FVector(0.0), FRotator(0.0f, 90.f, 0.0f), FName("Right Side Mesh"));
	SetupSideMesh(BackSideMeshComponent, BaseMesh, FVector(0.0), FRotator(0.0f, 180.f, 0.0f), FName("Back Side Mesh"));
	SetupSideMesh(TopSideMeshComponent, BaseMesh, FVector(0.0), FRotator(90.0f, 00.f, 0.0f), FName("Top Side Mesh"));
	SetupSideMesh(BottomSideMeshComponent, BaseMesh, FVector(0.0), FRotator(-90.0f, 0.f, 0.0f), FName("Bottom Side Mesh"));
}

void AFleshCube::SetupSideMesh(USkeletalMeshComponent*& MeshComponent, UStaticMeshComponent* ComponentParent, FVector ComponentLocation, FRotator ComponentRotation, FName ComponentName)
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(ComponentName);
	MeshComponent->AttachToComponent(ComponentParent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->SetRelativeLocation(ComponentLocation);
	MeshComponent->SetRelativeRotation(ComponentRotation);
}

// Called when the game starts or when spawned
void AFleshCube::BeginPlay()
{
	Super::BeginPlay();
	this->SetActorTickEnabled(false);
}

void AFleshCube::OnPickUp_Implementation(AActor* Caller, FVector ImpactPoint)
{
	AInteractableBase::OnPickUp_Implementation(Caller, ImpactPoint);
	//AInteractableBase::Execute_OnPickUp(this, Caller);
	bCurrentlyCarried = true;

	for (UFleshCubeSideBase* Side : ActivatedSides)
	{
		if (Side != nullptr)
		{
			Side->ReceivedStopSignal();
		}
	}

	ActivatedSides.Empty();

	if (LeftSide != nullptr)
	{
		LeftSide->ReceivedStopSignal();
	}
	if (FrontSide != nullptr)
	{
		FrontSide->ReceivedStopSignal();
	}
	if (RightSide != nullptr)
	{
		RightSide->ReceivedStopSignal();
	}
	if (BackSide != nullptr)
	{
		BackSide->ReceivedStopSignal();
	}
}

void AFleshCube::OnDropPickUp_Implementation(AActor* Caller)
{
	AInteractableBase::OnDropPickUp_Implementation(Caller);
	bCurrentlyCarried = false;

	bCanSendStartSignal = true;
	bHasLatched = false;
	this->SetActorTickEnabled(true);
}

// Called every frame
void AFleshCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult GroundCheckHitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(GroundCheckHitResult, this->GetActorLocation(), this->GetActorLocation() + (FVector::DownVector * 10000), ECC_Visibility, CollisionParams))
	{
		DrawDebugLine(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (FVector::DownVector * 10000), FColor::Magenta);
		if (FVector::Distance(this->GetActorLocation(), GroundCheckHitResult.ImpactPoint) < CubeGroundTraceDistance)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, TEXT("Found ground"));
			FHitResult CubeHitResult;
			TryToFindCubeNeighbour(CubeHitResult, LeftSideMeshComponent, CollisionParams, LeftSide, LeftSideType);
			TryToFindCubeNeighbour(CubeHitResult, FrontSideMeshComponent, CollisionParams, FrontSide, FrontSideType);
			TryToFindCubeNeighbour(CubeHitResult, RightSideMeshComponent, CollisionParams, RightSide, RightSideType);
			TryToFindCubeNeighbour(CubeHitResult, BackSideMeshComponent, CollisionParams, BackSide, BackSideType);

			bCanSendStartSignal = false;
			this->SetActorTickEnabled(false);
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
	}
}

void AFleshCube::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetupSides();
}

UFleshCubeSideBase* AFleshCube::GetCubeSideByComponentName(FString ColliderName)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, ColliderName);
	if (ColliderName.Contains(FString("Left")))
	{
		return LeftSide;
	}
	else if (ColliderName.Contains(FString("Front")))
	{
		return FrontSide;
	}
	else if (ColliderName.Contains(FString("Right")))
	{
		return RightSide;
	}
	else if (ColliderName.Contains(FString("Back")))
	{
		return BackSide;
	}
	else
	{
		return nullptr;
	}
}

void AFleshCube::ReceiveRemoteActivationSignal(FString ColliderName)
{
	UFleshCubeSideBase* CurrentSide = GetCubeSideByComponentName(ColliderName);

	if (CurrentSide != nullptr)
	{
		if (FaceData->SideData[CurrentSide->GetCurrentSideType()].bCanBeActivatedByPoop)
		{
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			FHitResult CubeHitResult;

			if (CurrentSide != LeftSide)
			{
				LeftSide->ReceivedActivationSignal(nullptr, ESideType::None, LeftSideMeshComponent->GetComponentToWorld());
			}
			if (CurrentSide != FrontSide)
			{
				FrontSide->ReceivedActivationSignal(nullptr, ESideType::None, FrontSideMeshComponent->GetComponentToWorld());
			}
			if (CurrentSide != RightSide)
			{
				RightSide->ReceivedActivationSignal(nullptr, ESideType::None, RightSideMeshComponent->GetComponentToWorld());
			}
			if (CurrentSide != BackSide)
			{
				BackSide->ReceivedActivationSignal(nullptr, ESideType::None, BackSideMeshComponent->GetComponentToWorld());
			}
		}
	}
}

//UFleshCubeSideBase* AFleshCube::GetCubeSideByMesh(FString MeshName)
//{
//	if (MeshName.Contains(FString("Left")))
//	{
//		return LeftSide;
//	}
//	else if (MeshName.Contains(FString("Front")))
//	{
//		return FrontSide;
//	}
//	else if (MeshName.Contains(FString("Right")))
//	{
//		return RightSide;
//	}
//	else if (MeshName.Contains(FString("Back")))
//	{
//		return BackSide;
//	}
//	else
//	{
//		return nullptr;
//	}
//}

void AFleshCube::SetupSides()
{
	SetupStartSides();

	if (FaceData == nullptr)
	{
		return;
	}

	SetupSide(LeftSideMeshComponent, LeftSideType, PreviousLeftSide, LeftSide);
	SetupSide(FrontSideMeshComponent, FrontSideType, PreviousFrontSide, FrontSide);
	SetupSide(RightSideMeshComponent, RightSideType, PreviousRightSide, RightSide);
	SetupSide(BackSideMeshComponent, BackSideType, PreviousBackSide, BackSide);
}

void AFleshCube::SetupStartSides()
{
	if (LeftSideType == ESideType::NoUse)
	{
		LeftSideType = ESideType::None;
	}
	if (FrontSideType == ESideType::NoUse)
	{
		FrontSideType = ESideType::None;
	}
	if (RightSideType == ESideType::NoUse)
	{
		RightSideType = ESideType::None;
	}
	if (BackSideType == ESideType::NoUse)
	{
		BackSideType = ESideType::None;
	}

	if (FaceData == nullptr)
	{
		return;
	}

	// Create a reference to none sidetype
	UFleshCubeSideBase* TemporaryNoneReference = NewObject<UFleshCubeSideBase>(this, FaceData->SideData[ESideType::None].Blueprint);

	// Make sure the blueprint for no face can be found
	if (TemporaryNoneReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find a blueprint to sidetype None"));
		return;
	}

	USkeletalMesh* TopAndBottomFaceMesh = TemporaryNoneReference->GetFaceMesh();
	UMaterialInstance* TopAndBottomMaterial = TemporaryNoneReference->GetFaceMaterial();

	// Set top and bottom meshes
	if (TopSideMeshComponent != nullptr)
	{
		TopSideMeshComponent->SetSkeletalMesh(TopAndBottomFaceMesh);
		TopSideMeshComponent->SetMaterial(0, TopAndBottomMaterial);
	}
	if (BottomSideMeshComponent != nullptr)
	{
		if (bCanPickup || BoltedMesh == nullptr)
		{
			BottomSideMeshComponent->SetSkeletalMesh(TopAndBottomFaceMesh);
			BottomSideMeshComponent->SetMaterial(0, TopAndBottomMaterial);
		}
		else
		{
			BottomSideMeshComponent->SetSkeletalMesh(BoltedMesh);
			if (BoltedMaterial != nullptr)
			{
				BottomSideMeshComponent->SetMaterial(0, BoltedMaterial);
				UE_LOG(LogTemp, Warning, TEXT("Bolted Material is not assigned"));
			}
		}
	}

	// Destroy this now unused component
	TemporaryNoneReference->DestroyComponent(false);

	bStartSidesGenerated = true;
}

void AFleshCube::TryToFindCubeNeighbour(FHitResult& CubeHitResult, USkeletalMeshComponent* MeshComponent, FCollisionQueryParams& CollisionParams, UFleshCubeSideBase* SendingSide, ESideType SideType)
{
	FVector StartLocation = MeshComponent->GetComponentLocation();
	FVector EndLocation = StartLocation + (MeshComponent->GetForwardVector() * CubeSideTraceDistance);
	if (GetWorld()->LineTraceSingleByChannel(CubeHitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		if (CubeHitResult.Actor->IsA<AFleshCube>())
		{
			AFleshCube* OtherCube = Cast<AFleshCube>(CubeHitResult.Actor);
			UFleshCubeSideBase* CurrentSide = OtherCube->GetCubeSideByComponentName(CubeHitResult.Component->GetName());

			if (CurrentSide != nullptr)
			{
				if (FaceData->SideData[CurrentSide->GetCurrentSideType()].FaceMatches.Contains(SideType))
				{
					OtherCube->SendActivationSignal(this, SendingSide, CurrentSide, SideType);
					ActivatedSides.Add(CurrentSide);
				}
				else
				{
					if (FaceData->SideData[SendingSide->GetCurrentSideType()].FaceMatches.Contains(CurrentSide->GetCurrentSideType()))
					{
						SendActivationSignal(OtherCube, CurrentSide, SendingSide, CurrentSide->GetCurrentSideType());
						OtherCube->ActivatedSides.Add(SendingSide);
					}
					else
					{
						const UEnum* SideTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESideType"));
						UE_LOG(LogTemp, Warning, TEXT("Sending side %s"), *SideTypeEnum->GetEnumName((int32)SendingSide->GetCurrentSideType()));
						UE_LOG(LogTemp, Warning, TEXT("Recieving side %s"), *SideTypeEnum->GetEnumName((int32)CurrentSide->GetCurrentSideType()));
					}
				}
				if (bHasLatched == false)
				{
					LatchCube(MeshComponent, Cast<UPrimitiveComponent>(CubeHitResult.Component));
				}
			}
		}
	}
}

void AFleshCube::SetupSide(USkeletalMeshComponent*& SideMeshComponent, ESideType& SideType, ESideType& PreviousType, UFleshCubeSideBase*& CubeSide)
{
	if (SideType != PreviousType)
	{
		TSubclassOf<UFleshCubeSideBase> CubeObject = nullptr;

		if (!FaceData->SideData.Contains(SideType))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find the type in sidedata"));
			SideType = ESideType::None;
			SetupSide(SideMeshComponent, SideType, PreviousType, CubeSide);
			return;
		}
		else
		{
			CubeObject = FaceData->SideData[SideType].Blueprint;
		}

		if (CubeSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			SideMeshComponent->SetSkeletalMesh(nullptr);
			CubeSide->bEditableWhenInherited = false;
			CubeSide->UnregisterComponent();
			CubeSide->DestroyComponent(false);
		}

		if (CubeObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("CubeObject is nullptr"));
			SideMeshComponent->SetSkeletalMesh(nullptr);
			CubeSide = nullptr;
		}
		else
		{
			UFleshCubeSideBase* FleshCubeSide = NewObject<UFleshCubeSideBase>(this, FaceData->SideData[SideType].Blueprint);

			if (FleshCubeSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			CubeSide = FleshCubeSide;
			CubeSide->RegisterComponent();
			CubeSide->bEditableWhenInherited = false;

			USkeletalMesh* MeshToUse = CubeSide->GetFaceMesh();
			UMaterialInstance* MaterialToUse = CubeSide->GetFaceMaterial();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from MeshSide"));
			}

			if (SideMeshComponent == nullptr || CubeSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			SideMeshComponent->SetSkeletalMesh(MeshToUse);
			SideMeshComponent->SetMaterial(0, MaterialToUse);
		}

		CubeSide->SetCurrentSideType(SideType);
		PreviousType = SideType;
	}
}

void AFleshCube::LatchCube(USkeletalMeshComponent* Start, UPrimitiveComponent* CubeSide)
{
	bHasLatched = true;

	FVector StartLocation = Start->GetComponentLocation();
	FVector StartForward = Start->GetForwardVector();
	StartForward.Normalize();

	FVector HitLocation = CubeSide->GetComponentLocation();
	FVector HitForward = CubeSide->GetForwardVector();
	HitForward.Normalize();

	float Distance = FVector::Distance(StartLocation, HitLocation);
	FVector NewLocation = HitLocation + (HitForward * Distance);
	SetActorLocation(NewLocation);

	float Radiants = FMath::Acos(FVector::DotProduct(-StartForward, HitForward));
	float Angle = FMath::RadiansToDegrees(FVector::DotProduct(Start->GetRightVector(), HitForward) < 0 ? Radiants : -Radiants);
	FRotator NewRotation = { 0, Angle, 0 };
	SetActorRotation(GetActorRotation() + NewRotation);
}