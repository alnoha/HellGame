// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCube.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFleshCube::AFleshCube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ReadFaceData();

	SetupBaseMesh();
	SetupSideMeshes();
}

void AFleshCube::OnSideCollisionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AFleshCube::OnSideCollisionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
}

void AFleshCube::SendActivationSignal(AFleshCube* SendingCube, UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType, bool ReturnSignal)
{
	if (!FaceData->SideData.Contains(ReceivingSide->GetCurrentSideType()))
	{
		const UEnum* SideTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESideType"));
		UE_LOG(LogTemp, Warning, TEXT("Can't find receiving side in data asset %s"), *SideTypeEnum->GetEnumName((int32)ReceivingSide->GetCurrentSideType()));
		return;
	}

	if (FaceData->SideData[ReceivingSide->GetCurrentSideType()].FaceMatches.Contains(SendingType))
	{
		if (ReceivingSide == LeftSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("LeftMatch"));
			FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
			RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
			BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
		}
		else if (ReceivingSide == FrontSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("FrontMatch"));
			RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
			BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
			LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
		}
		else if (ReceivingSide == RightSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("RightMatch"));
			BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
			LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
			FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
		}
		else if (ReceivingSide == BackSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("BackMatch"));
			LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
			FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
			RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
		}
	}
}

void AFleshCube::ReadFaceData()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> CubeSideBase(TEXT("CubeFaceDataold_C'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/CubeFaceData.CubeFaceData'"));

	if (CubeSideBase.Succeeded())
	{
		FaceData = Cast<UCubeFaceData>(CubeSideBase.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find reference"));
	}
}

void AFleshCube::SetupBaseMesh()
{
	// Create base cube mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxBase"));
	//BaseMesh->RegisterComponent();

	this->SetRootComponent(BaseMesh);

	// Setup Physics
	BaseMesh->SetSimulatePhysics(true);
	//BaseMesh->SetMassOverrideInKg(FName("None"), 900.0f, true);
}

void AFleshCube::SetupSideMeshes()
{
	FVector BoxExtent = FVector(50.0f, 70.0f, 70.0f);
	FVector BoxLocation = FVector(120.0f, 0.0f, 0.0f);

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

void AFleshCube::OnPickUp_Implementation(AActor* Caller)
{
	AInteractableBase::OnPickUp_Implementation(Caller);
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
	this->SetActorTickEnabled(true);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Setting up drop"));
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
		if (FVector::Distance(this->GetActorLocation(), GroundCheckHitResult.ImpactPoint) < 77.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Found ground"));

			// Send out traces from every side

			FHitResult CubeHitResult;
			if (GetWorld()->LineTraceSingleByChannel(CubeHitResult, LeftSideMeshComponent->GetComponentLocation(), (LeftSideMeshComponent->GetComponentLocation() + (LeftSideMeshComponent->GetForwardVector() * CubeSideTraceDistance)), ECC_Visibility, CollisionParams))
			{
				if (CubeHitResult.Actor->IsA<AFleshCube>())
				{
					AFleshCube* OtherCube = Cast<AFleshCube>(CubeHitResult.Actor);
					UFleshCubeSideBase* CurrentSide = OtherCube->GetCubeSideByMesh(CubeHitResult.Component->GetName());

					if (CurrentSide != nullptr)
					{
						OtherCube->SendActivationSignal(this, LeftSide, CurrentSide, LeftSideType);
						ActivatedSides.Add(CurrentSide);
						//LatchCube(CubeHitResult.TraceStart, Cast<UPrimitiveComponent>(CubeHitResult.Component));
					}
				}
			}

			if (GetWorld()->LineTraceSingleByChannel(CubeHitResult, FrontSideMeshComponent->GetComponentLocation(), (FrontSideMeshComponent->GetComponentLocation() + (FrontSideMeshComponent->GetForwardVector() * CubeSideTraceDistance)), ECC_Visibility, CollisionParams))
			{
				if (CubeHitResult.Actor->IsA<AFleshCube>())
				{
					AFleshCube* OtherCube = Cast<AFleshCube>(CubeHitResult.Actor);
					UFleshCubeSideBase* CurrentSide = OtherCube->GetCubeSideByMesh(CubeHitResult.Component->GetName());

					if (CurrentSide != nullptr)
					{
						OtherCube->SendActivationSignal(this, FrontSide, CurrentSide, FrontSideType);
						ActivatedSides.Add(CurrentSide);
						//LatchCube(CubeHitResult.TraceStart, Cast<UPrimitiveComponent>(CubeHitResult.Component));
					}
				}
			}

			if (GetWorld()->LineTraceSingleByChannel(CubeHitResult, RightSideMeshComponent->GetComponentLocation(), (RightSideMeshComponent->GetComponentLocation() + (RightSideMeshComponent->GetForwardVector() * CubeSideTraceDistance)), ECC_Visibility, CollisionParams))
			{
				if (CubeHitResult.Actor->IsA<AFleshCube>())
				{
					AFleshCube* OtherCube = Cast<AFleshCube>(CubeHitResult.Actor);
					UFleshCubeSideBase* CurrentSide = OtherCube->GetCubeSideByMesh(CubeHitResult.Component->GetName());

					if (CurrentSide != nullptr)
					{
						OtherCube->SendActivationSignal(this, RightSide, CurrentSide, RightSideType);
						ActivatedSides.Add(CurrentSide);
						//LatchCube(CubeHitResult.TraceStart, Cast<UPrimitiveComponent>(CubeHitResult.Component));
					}
				}
			}

			if (GetWorld()->LineTraceSingleByChannel(CubeHitResult, BackSideMeshComponent->GetComponentLocation(), (BackSideMeshComponent->GetComponentLocation() + (BackSideMeshComponent->GetForwardVector() * CubeSideTraceDistance)), ECC_Visibility, CollisionParams))
			{
				if (CubeHitResult.Actor->IsA<AFleshCube>())
				{
					AFleshCube* OtherCube = Cast<AFleshCube>(CubeHitResult.Actor);
					UFleshCubeSideBase* CurrentSide = OtherCube->GetCubeSideByMesh(CubeHitResult.Component->GetName());

					if (CurrentSide != nullptr)
					{
						OtherCube->SendActivationSignal(this, BackSide, CurrentSide, BackSideType);
						ActivatedSides.Add(CurrentSide);
						//LatchCube(CubeHitResult.TraceStart, Cast<UPrimitiveComponent>(CubeHitResult.Component));
					}
				}
			}

			bCanSendStartSignal = false;
			this->SetActorTickEnabled(false);
		}
	}
}

void AFleshCube::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetupSides();

	/*LeftSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);
	FrontSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);
	RightSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);
	BackSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);


	LeftSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);
	FrontSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);
	RightSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);
	BackSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);*/

}

UFleshCubeSideBase* AFleshCube::GetCubeSideByCollider(FString ColliderName)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, ColliderName);
	if (ColliderName.Contains(FString("Left")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("left side"));
		return LeftSide;
	}
	else if (ColliderName.Contains(FString("Front")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("front side"));
		return FrontSide;
	}
	else if (ColliderName.Contains(FString("Right")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("right side"));
		return RightSide;
	}
	else if (ColliderName.Contains(FString("Back")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("back side"));
		return BackSide;
	}
	else
	{
		return nullptr;
	}
}

UFleshCubeSideBase* AFleshCube::GetCubeSideByMesh(FString MeshName)
{
	if (MeshName.Contains(FString("Left")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("left side"));
		return LeftSide;
	}
	else if (MeshName.Contains(FString("Front")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("front side"));
		return FrontSide;
	}
	else if (MeshName.Contains(FString("Right")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("right side"));
		return RightSide;
	}
	else if (MeshName.Contains(FString("Back")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("back side"));
		return BackSide;
	}
	else
	{
		return nullptr;
	}
}

void AFleshCube::SetupSides()
{
	if (!bStartSidesGenerated)
	{
		if (TopSideMeshComponent == nullptr || BottomSideMeshComponent == nullptr)
		{
			return;
		}

		SetupStartSides();
	}

	if (FaceData == nullptr)
	{
		return;
	}

	SetupSide(LeftSideMeshComponent, LeftSideType, PreviousLeftSide, LeftSide);
	SetupSide(FrontSideMeshComponent, FrontSideType, PreviousFrontSide, FrontSide);
	SetupSide(RightSideMeshComponent, RightSideType, PreviousRightSide, RightSide);
	SetupSide(BackSideMeshComponent, BackSideType, PreviousBackSide, BackSide);
	// SetupLeftSide();
	// SetupFrontSide();
	// SetupRightSide();
	// SetupBackSide();
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
	auto x = NewObject<UFleshCubeSideBase>(this, FaceData->SideData[ESideType::None].Blueprint);

	// Make sure the blueprint for no face can be found
	if (x == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find a blueprint to sidetype None"));
		return;
	}

	// Set top and bottom meshes
	TopSideMeshComponent->SetSkeletalMesh(x->GetFaceMesh());
	BottomSideMeshComponent->SetSkeletalMesh(x->GetFaceMesh());

	// Destroy this now unused component
	x->DestroyComponent(false);

	bStartSidesGenerated = true;
}

void AFleshCube::SetupSide(USkeletalMeshComponent*& SideMeshComponent, ESideType& SideType, ESideType& PreviousType, UFleshCubeSideBase*& CubeSide)
{
	if (SideType != PreviousType)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing side on FleshCube"));

		TSubclassOf<UFleshCubeSideBase> MyObject = nullptr;

		if (!FaceData->SideData.Contains(SideType))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find the type in sidedata"));
			SideType = ESideType::None;
			SetupSide(SideMeshComponent, SideType, PreviousType, CubeSide);
			return;
		}
		else
		{
			MyObject = FaceData->SideData[SideType].Blueprint;
		}

		// auto MyObject = FaceData->SideData[SideType].Blueprint;

		if (CubeSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			SideMeshComponent->SetSkeletalMesh(nullptr);
			CubeSide->bEditableWhenInherited = false;
			CubeSide->UnregisterComponent();
			CubeSide->DestroyComponent(false);
		}

		if (MyObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyObject is nullptr"));
			SideMeshComponent->SetSkeletalMesh(nullptr);
			CubeSide = nullptr;
		}
		else
		{
			auto x = NewObject<UFleshCubeSideBase>(this, FaceData->SideData[SideType].Blueprint);

			if (x == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			CubeSide = x;
			CubeSide->RegisterComponent();
			CubeSide->bEditableWhenInherited = false;

			USkeletalMesh* MeshToUse = CubeSide->GetFaceMesh();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from leftside"));
			}

			if (SideMeshComponent == nullptr || CubeSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			SideMeshComponent->SetSkeletalMesh(CubeSide->GetFaceMesh());
		}

		CubeSide->SetCurrentSideType(SideType);
		PreviousType = SideType;
	}
}

void AFleshCube::LatchCube(FVector Start, UPrimitiveComponent* CubeSide)
{
	FVector StartForward = Start.ForwardVector;
	FVector CubeSideForward = CubeSide->GetForwardVector();

	FVector NewLocation = CubeSide->GetComponentLocation() + (CubeSideForward * CubeSnapDistance);
	this->SetActorLocation(NewLocation);

	FQuat StartQuat = Start.ToOrientationRotator().Quaternion();
	FQuat EndQuat = CubeSide->GetComponentRotation().Quaternion();
	this->SetActorRotation(EndQuat * StartQuat);
}