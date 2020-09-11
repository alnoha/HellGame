// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCube.h"
#include "DrawDebugHelpers.h"

AFleshCube::AFleshCube()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupBaseMesh();
	SetupSideMeshes();
}

#pragma region Unreal Methods

void AFleshCube::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetupSides();
}

void AFleshCube::BeginPlay()
{
	Super::BeginPlay();
	this->SetActorTickEnabled(false);

	LeftSide->Initialize_Side(EyeDataLeftSide, SnotScaleLeftSide);
	FrontSide->Initialize_Side(EyeDataFrontSide, SnotScaleFrontSide);
	RightSide->Initialize_Side(EyeDataRightSide, SnotScaleRightSide);
	BackSide->Initialize_Side(EyeDataBackSide, SnotScaleBackSide);
}

void AFleshCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult GroundCheckHitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(GroundCheckHitResult, this->GetActorLocation(), this->GetActorLocation() + (FVector::DownVector * 10000), ECC_Visibility, CollisionParams))
	{
		if (FVector::Distance(this->GetActorLocation(), GroundCheckHitResult.ImpactPoint) < CubeGroundTraceDistance)
		{
			HitGround();
			FHitResult CubeHitResult;
			TryToFindCubeNeighbour(CubeHitResult, LeftSideMeshComponent, CollisionParams, LeftSide, LeftSideType);
			TryToFindCubeNeighbour(CubeHitResult, FrontSideMeshComponent, CollisionParams, FrontSide, FrontSideType);
			TryToFindCubeNeighbour(CubeHitResult, RightSideMeshComponent, CollisionParams, RightSide, RightSideType);
			TryToFindCubeNeighbour(CubeHitResult, BackSideMeshComponent, CollisionParams, BackSide, BackSideType);

			bCanSendStartSignal = false;
			this->SetActorTickEnabled(false);

			FBodyInstance* Componenet = Cast<UPrimitiveComponent>(this->GetRootComponent())->GetBodyInstance();
			Componenet->bLockXTranslation = true;
			Componenet->bLockYTranslation = true;

			Componenet->bLockXRotation = true;
			Componenet->bLockYRotation = true;
			Componenet->bLockZRotation = true;
			Componenet->SetDOFLock(EDOFMode::SixDOF);
		}
	}
}

#pragma endregion

#pragma region Event Implementations

void AFleshCube::HitGround_Implementation()
{
}

void AFleshCube::OnPickUp_Implementation(AActor* Caller, UPrimitiveComponent* ImpactPoint)
{
	AInteractableBase::OnPickUp_Implementation(Caller, ImpactPoint);

	bCurrentlyCarried = true;
	bCanSendStartSignal = false;
	SetActorTickEnabled(false);

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
	SetActorTickEnabled(true);
}

#pragma endregion

#pragma region Setup Sides And Base Mesh

#pragma region Meshes
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
#pragma endregion

#pragma region Sides
void AFleshCube::SetupSides()
{
	SetupStartSides();

	if (FaceData == nullptr)
	{
		return;
	}

	SetupSide(LeftSideMeshComponent, LeftSideType, PreviousLeftSide, LeftSide, EyeDataLeftSide, SnotScaleLeftSide);
	SetupSide(FrontSideMeshComponent, FrontSideType, PreviousFrontSide, FrontSide, EyeDataFrontSide, SnotScaleFrontSide);
	SetupSide(RightSideMeshComponent, RightSideType, PreviousRightSide, RightSide, EyeDataRightSide, SnotScaleRightSide);
	SetupSide(BackSideMeshComponent, BackSideType, PreviousBackSide, BackSide, EyeDataBackSide, SnotScaleRightSide);
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

void AFleshCube::SetupSide(USkeletalMeshComponent*& SideMeshComponent, ESideType& SideType, ESideType& PreviousType, UFleshCubeSideBase*& CubeSide, FEyeComponentData EyeComponentData, FVector SnotScale)
{
	if (SideType != PreviousType)
	{
		TSubclassOf<UFleshCubeSideBase> CubeObject = nullptr;

		if (!FaceData->SideData.Contains(SideType))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find the type in sidedata"));
			SideType = ESideType::None;
			SetupSide(SideMeshComponent, SideType, PreviousType, CubeSide, EyeComponentData, SnotScale);
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
		CubeSide->Initialize_Side(EyeComponentData, SnotScale);
		PreviousType = SideType;
	}
}
#pragma endregion

#pragma endregion

#pragma region Activation Signals

void AFleshCube::SendActivationSignal(AFleshCube* SendingCube, UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType, bool ReturnSignal)
{
	bool bFoundaSide = false;

	if (ReceivingSide != LeftSide)
	{
		LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->SkeletalMesh, LeftSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (ReceivingSide != FrontSide)
	{
		FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->SkeletalMesh, FrontSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (ReceivingSide != RightSide)
	{
		RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->SkeletalMesh, RightSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (ReceivingSide != BackSide)
	{
		BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->SkeletalMesh, BackSideMeshComponent->GetComponentToWorld());
		bFoundaSide = true;
	}

	if (!bFoundaSide)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Could not find a side"));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, ReceivingSide->GetName());
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
				LeftSide->ReceivedActivationSignal(nullptr, ESideType::None, LeftSideMeshComponent->SkeletalMesh, LeftSideMeshComponent->GetComponentToWorld());
			}

			if (CurrentSide != FrontSide)
			{
				FrontSide->ReceivedActivationSignal(nullptr, ESideType::None, FrontSideMeshComponent->SkeletalMesh, FrontSideMeshComponent->GetComponentToWorld());
			}

			if (CurrentSide != RightSide)
			{
				RightSide->ReceivedActivationSignal(nullptr, ESideType::None, RightSideMeshComponent->SkeletalMesh, RightSideMeshComponent->GetComponentToWorld());
			}

			if (CurrentSide != BackSide)
			{
				BackSide->ReceivedActivationSignal(nullptr, ESideType::None, BackSideMeshComponent->SkeletalMesh, BackSideMeshComponent->GetComponentToWorld());
			}
		}
	}
}

#pragma endregion

#pragma region Cube Placement

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

/*Latch a cube to an other cube, LatchingCube = Cube that will get the changes CubeSide = the cube that will give the values to Latch to.*/
void AFleshCube::LatchCube(USkeletalMeshComponent* LatchingCube, UPrimitiveComponent* CubeSide)
{
	bHasLatched = true;

	FVector StartLocation = LatchingCube->GetComponentLocation();
	FVector StartForward = LatchingCube->GetForwardVector();
	StartForward.Normalize();

	FVector HitLocation = CubeSide->GetComponentLocation();
	FVector HitForward = CubeSide->GetForwardVector();
	HitForward.Normalize();

	float Distance = FVector::Distance(StartLocation, HitLocation);
	FVector NewLocation = HitLocation + (HitForward * Distance);
	SetActorLocation(NewLocation);

	float Radiants = FMath::Acos(FVector::DotProduct(-StartForward, HitForward));
	float Degree = FMath::RadiansToDegrees(FVector::DotProduct(LatchingCube->GetRightVector(), HitForward) < 0 ? Radiants : -Radiants);
	SetActorRotation(GetActorRotation() + FRotator(0.0f, Degree, 0.0f));
}

#pragma endregion

UFleshCubeSideBase* AFleshCube::GetCubeSideByComponentName(FString ColliderName)
{
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