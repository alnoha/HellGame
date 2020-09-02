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

	// Temporary holders until I get help regarding data tables

	// Mouth:
	TemporaryReferenceFiller(ESideType::Mouth, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_MouthFace.BP_MouthFace'"));
	TemporaryReferenceFiller(ESideType::None, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_NoneFace.BP_NoneFace'"));
	TemporaryReferenceFiller(ESideType::Nose, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_NoseFace.BP_NoseFace'"));
	TemporaryReferenceFiller(ESideType::Butt, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_ButtFace.BP_ButtFace'"));
	TemporaryReferenceFiller(ESideType::Eye, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_EyeFace.BP_EyeFace'"));
	TemporaryReferenceFiller(ESideType::Hair, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_HairFace.BP_HairFace'"));
	TemporaryReferenceFiller(ESideType::Ear, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/CPPFleshCube/BP_EarFace.BP_EarFace'"));

	// End temporary holders
}

void AFleshCube::TemporaryReferenceFiller(ESideType SideType, const TCHAR* Reference)
{
	ConstructorHelpers::FObjectFinder<UBlueprint> CubeSideBase(Reference);
	if (CubeSideBase.Succeeded())
	{
		BlueprintFaces.Add(SideType, (UClass*)CubeSideBase.Object->GeneratedClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find reference: %i"), Reference);
	}
}

bool AFleshCube::HasConnectedNeighbour()
{
	return LeftConnectedCube.ConnectedCube != nullptr
		|| RightConnectedCube.ConnectedCube != nullptr
		|| FrontConnectedCube.ConnectedCube != nullptr
		|| BackConnectedCube.ConnectedCube != nullptr;
}

void AFleshCube::OnSideCollisionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AFleshCube>())
	{
		AFleshCube* OtherCube = Cast<AFleshCube>(OtherActor);
		UBoxComponent* OtherCollider = Cast<UBoxComponent>(OverlappedComp);

		UFleshCubeSideBase* OtherCubeSide = OtherCube->GetCubeSideByCollider(OtherComp->GetName());

		if (OtherCubeSide == nullptr)
		{
			return;
		}

		if (OverlappedComp->GetName().Equals(LeftSideBoxCollider->GetName()))
		{
			LeftConnectedCube = FConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
		else if (OverlappedComp->GetName().Equals(FrontSideBoxCollider->GetName()))
		{
			FrontConnectedCube = FConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
		else if (OverlappedComp->GetName().Equals(RightSideBoxCollider->GetName()))
		{
			RightConnectedCube = FConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
		else if (OverlappedComp->GetName().Equals(BackSideBoxCollider->GetName()))
		{
			BackConnectedCube = FConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
	}
}

void AFleshCube::OnSideCollisionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AFleshCube>())
	{
		if (OverlappedComp == LeftSideBoxCollider)
		{
			LeftConnectedCube = FConnectedCubeInfo(nullptr, nullptr);
		}
		else if (OverlappedComp == FrontSideBoxCollider)
		{
			FrontConnectedCube = FConnectedCubeInfo(nullptr, nullptr);
		}
		else if (OverlappedComp == RightSideBoxCollider)
		{
			RightConnectedCube = FConnectedCubeInfo(nullptr, nullptr);
		}
		else if (OverlappedComp == BackSideBoxCollider)
		{
			BackConnectedCube = FConnectedCubeInfo(nullptr, nullptr);
		}
	}
}

void AFleshCube::SendActivationSignal(AFleshCube* SendingCube, UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType, bool ReturnSignal)
{
	bool FaceMatch = false;
	if (SendingType == ESideType::Butt)
	{
		if (ReceivingSide->GetCurrentSideType() == ESideType::Nose)
		{
			FaceMatch = true;
		}
	}
	else if(SendingType == ESideType::Nose)
	{
		if (ReceivingSide->GetCurrentSideType() == ESideType::Eye)
		{
			FaceMatch = true;
		}
	}
	else if (SendingType == ESideType::Eye)
	{
		if (ReceivingSide->GetCurrentSideType() == ESideType::Mouth)
		{
			FaceMatch = true;
		}
	}
	else if (SendingType == ESideType::Mouth)
	{
		if (ReceivingSide->GetCurrentSideType() == ESideType::Butt)
		{
			FaceMatch = true;
		}
	}

	if (FaceMatch)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("FaceMatch"));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, ReceivingSide->GetName());
		if (ReceivingSide == LeftSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("LeftMatch"));
			FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
			LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
			BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
		}
		else if (ReceivingSide == FrontSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("FrontMatch"));
			RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
			FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
			LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
		}
		else if (ReceivingSide == RightSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("RightMatch"));
			BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
			RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
			FrontSide->ReceivedActivationSignal(SendingSide, SendingType, FrontSideMeshComponent->GetComponentToWorld());
		}
		else if (ReceivingSide == BackSide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("BackMatch"));
			LeftSide->ReceivedActivationSignal(SendingSide, SendingType, LeftSideMeshComponent->GetComponentToWorld());
			BackSide->ReceivedActivationSignal(SendingSide, SendingType, BackSideMeshComponent->GetComponentToWorld());
			RightSide->ReceivedActivationSignal(SendingSide, SendingType, RightSideMeshComponent->GetComponentToWorld());
		}
	}
	/*else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("No FaceMatch"));
		if (!ReturnSignal)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("No Returnsignal"));
			SendingCube->SendActivationSignal(this, ReceivingSide, SendingSide, ReceivingSide->GetCurrentSideType(), true);
		}
	}*/
}

void AFleshCube::SetupBaseMesh()
{
	// Create base cube mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxBase"));
	BaseMesh->RegisterComponent();

	this->SetRootComponent(BaseMesh);

	// Setup Physics
	BaseMesh->SetSimulatePhysics(true);
	BaseMesh->SetMassOverrideInKg(FName("None"), 900.0f, true);
}

void AFleshCube::SetupSideMeshes()
{
	FVector BoxExtent = FVector(50.0f, 70.0f, 70.0f);
	FVector BoxLocation = FVector(-130.0f, 0.0f, 0.0f);

	LeftSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Left Side Mesh");
	LeftSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	LeftSideMeshComponent->RegisterComponent();
	LeftSideMeshComponent->bEditableWhenInherited = true;
	LeftSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LeftSideMeshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	LeftSideBoxCollider = CreateDefaultSubobject<UBoxComponent>("Left Box collider");
	LeftSideBoxCollider->bEditableWhenInherited = true;
	LeftSideBoxCollider->AttachToComponent(LeftSideMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LeftSideBoxCollider->RegisterComponent();
	LeftSideBoxCollider->SetBoxExtent(BoxExtent);
	LeftSideBoxCollider->SetRelativeLocation(BoxLocation);

	FrontSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Front Side Mesh");
	FrontSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	FrontSideMeshComponent->RegisterComponent();
	FrontSideMeshComponent->bEditableWhenInherited = true;
	FrontSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FrontSideMeshComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	FrontSideBoxCollider = CreateDefaultSubobject<UBoxComponent>("Front Box collider");
	FrontSideBoxCollider->bEditableWhenInherited = true;
	FrontSideBoxCollider->AttachToComponent(FrontSideMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FrontSideBoxCollider->RegisterComponent();
	FrontSideBoxCollider->SetBoxExtent(BoxExtent);
	FrontSideBoxCollider->SetRelativeLocation(BoxLocation);

	RightSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Right Side Mesh");
	RightSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	RightSideMeshComponent->RegisterComponent();
	RightSideMeshComponent->bEditableWhenInherited = true;
	RightSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RightSideMeshComponent->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	RightSideBoxCollider = CreateDefaultSubobject<UBoxComponent>("Right Box collider");
	RightSideBoxCollider->bEditableWhenInherited = true;
	RightSideBoxCollider->AttachToComponent(RightSideMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	RightSideBoxCollider->RegisterComponent();
	RightSideBoxCollider->SetBoxExtent(BoxExtent);
	RightSideBoxCollider->SetRelativeLocation(BoxLocation);

	BackSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Back Side Mesh");
	BackSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BackSideMeshComponent->bEditableWhenInherited = true;
	BackSideMeshComponent->RegisterComponent();
	BackSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	BackSideBoxCollider = CreateDefaultSubobject<UBoxComponent>("Back Box collider");
	BackSideBoxCollider->bEditableWhenInherited = true;
	BackSideBoxCollider->AttachToComponent(BackSideMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BackSideBoxCollider->RegisterComponent();
	BackSideBoxCollider->SetBoxExtent(BoxExtent);
	BackSideBoxCollider->SetRelativeLocation(BoxLocation);

	TopSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Top Side Mesh");
	TopSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TopSideMeshComponent->bEditableWhenInherited = true;
	TopSideMeshComponent->RegisterComponent();
	TopSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TopSideMeshComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	BottomSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Bottom Side Mesh");
	BottomSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BottomSideMeshComponent->bEditableWhenInherited = true;
	BottomSideMeshComponent->RegisterComponent();
	BottomSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BottomSideMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
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
	bCurrentlyCarried = true;

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

			if (LeftConnectedC != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Draw left line"));
			}
			if (FrontConnectedC != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Draw front line"));
			}
			if (RightConnectedC != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Draw right line"));
			}
			if (BackConnectedC != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Draw back line"));
			}

			if (LeftConnectedCube.ConnectedCube != nullptr)
			{
				if (LeftConnectedCube.ConnectedFace == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("left = null"));

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("left != null"));
					LeftConnectedCube.ConnectedCube->SendActivationSignal(this, LeftSide, LeftConnectedCube.ConnectedFace, LeftSideType);
				}
			}

			if (FrontConnectedCube.ConnectedCube != nullptr)
			{
				if (FrontConnectedCube.ConnectedFace == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("front = null"));

				}
				else
				{
					FrontConnectedCube.ConnectedCube->SendActivationSignal(this, FrontSide, FrontConnectedCube.ConnectedFace, FrontSideType);
				}
			}

			if (RightConnectedCube.ConnectedCube != nullptr)
			{
				if (RightConnectedCube.ConnectedFace == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("right = null"));

				}
				else
				{
					RightConnectedCube.ConnectedCube->SendActivationSignal(this, RightSide, RightConnectedCube.ConnectedFace, RightSideType);
				}
			}

			if (BackConnectedCube.ConnectedCube != nullptr)
			{
				if (BackConnectedCube.ConnectedFace == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("back = null"));

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("back != null"));
					BackConnectedCube.ConnectedCube->SendActivationSignal(this, BackSide, BackConnectedCube.ConnectedFace, BackSideType);
				}
			}


			bCanSendStartSignal = false;
			this->SetActorTickEnabled(false);
		}
	}
}

void AFleshCube::OnConstruction(const FTransform& Transform)
{
	SetupSides();

	LeftSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);
	FrontSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);
	RightSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);
	BackSideBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionEnter);


	LeftSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);
	FrontSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);
	RightSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);
	BackSideBoxCollider->OnComponentEndOverlap.AddDynamic(this, &AFleshCube::OnSideCollisionExit);

	Super::OnConstruction(Transform);
}

UFleshCubeSideBase* AFleshCube::GetCubeSideByCollider(FString ColliderName)
{
	if (ColliderName.Equals(LeftSideBoxCollider->GetName()))
	{
		return LeftSide;
	}
	else if (ColliderName.Equals(FrontSideBoxCollider->GetName()))
	{
		return FrontSide;
	}
	else if (ColliderName.Equals(RightSideBoxCollider->GetName()))
	{
		return RightSide;
	}
	else if (ColliderName.Equals(BackSideBoxCollider->GetName()))
	{
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
		SetupStartSides();
	}
	// SetupSide(LeftSideMeshComponent, LeftSideType, PreviousLeftSide, LeftSide);
	// SetupSide(FrontSideMeshComponent, FrontSideType, PreviousFrontSide, FrontSide);
	// SetupSide(RightSideMeshComponent, RightSideType, PreviousRightSide, RightSide);
	// SetupSide(BackSideMeshComponent, BackSideType, PreviousBackSide, BackSide);
	SetupLeftSide();
	SetupFrontSide();
	SetupRightSide();
	SetupBackSide();
}

void AFleshCube::SetupStartSides()
{
	// Create a reference to none sidetype
	auto x = NewObject<UFleshCubeSideBase>(this, BlueprintFaces.FindRef(ESideType::None));

	// Make sure the blueprint for no face can be found
	if (x == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find a blueprint to sidetype None"));
		return;
	}

	// Set top and bottom meshes
	TopSideMeshComponent->SetStaticMesh(x->GetFaceMesh());
	BottomSideMeshComponent->SetStaticMesh(x->GetFaceMesh());

	// Set side meshes
	LeftSideMeshComponent->SetStaticMesh(x->GetFaceMesh());
	FrontSideMeshComponent->SetStaticMesh(x->GetFaceMesh());
	RightSideMeshComponent->SetStaticMesh(x->GetFaceMesh());
	BackSideMeshComponent->SetStaticMesh(x->GetFaceMesh());
	
	// Destroy this now unused component
	x->DestroyComponent(false);

	// Make sure we only do this once since it's in the OnConstruction method
	bStartSidesGenerated = true;
}

void AFleshCube::SetupSide(UStaticMeshComponent* SideMeshComponent, ESideType SideType, ESideType& PreviousType, UFleshCubeSideBase* CubeSide)
{
	if (SideType != PreviousType)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing side on FleshCube"));

		auto MyObject = BlueprintFaces.Find(SideType);

		if (CubeSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			SideMeshComponent->SetStaticMesh(nullptr);
			CubeSide->bEditableWhenInherited = false;
			CubeSide->UnregisterComponent();
			CubeSide->DestroyComponent(false);
		}

		if (MyObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyObject is nullptr"));
			SideMeshComponent->SetStaticMesh(nullptr);
			CubeSide = nullptr;
		}
		else
		{
			auto x = NewObject<UFleshCubeSideBase>(this, BlueprintFaces.FindRef(SideType));

			if (x == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			CubeSide = x;
			CubeSide->RegisterComponent();
			CubeSide->bEditableWhenInherited = false;

			UStaticMesh* MeshToUse = CubeSide->GetFaceMesh();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from leftside"));
			}

			if (SideMeshComponent == nullptr || CubeSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			SideMeshComponent->SetStaticMesh(CubeSide->GetFaceMesh());
		}

		PreviousType = SideType;
	}
}

void AFleshCube::SetupLeftSide()
{
	if (LeftSideType != PreviousLeftSide)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing side on FleshCube"));

		auto MyObject = BlueprintFaces.Find(LeftSideType);

		if (LeftSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			LeftSideMeshComponent->SetStaticMesh(nullptr);
			LeftSide->bEditableWhenInherited = false;
			LeftSide->UnregisterComponent();
			LeftSide->DestroyComponent(false);
		}

		if (MyObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyObject is nullptr"));
			LeftSideMeshComponent->SetStaticMesh(nullptr);
			LeftSide = nullptr;
		}
		else
		{
			auto x = NewObject<UFleshCubeSideBase>(this, BlueprintFaces.FindRef(LeftSideType));

			if (x == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			LeftSide = x;
			LeftSide->RegisterComponent();
			LeftSide->bEditableWhenInherited = false;

			UStaticMesh* MeshToUse = LeftSide->GetFaceMesh();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from leftside"));
			}

			if (LeftSideMeshComponent == nullptr || LeftSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			LeftSideMeshComponent->SetStaticMesh(LeftSide->GetFaceMesh());
		}

		LeftSide->SetCurrentSideType(LeftSideType);
		PreviousLeftSide = LeftSideType;
	}
}

void AFleshCube::SetupFrontSide()
{
	if (FrontSideType != PreviousFrontSide)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing side on FleshCube"));

		auto MyObject = BlueprintFaces.Find(FrontSideType);

		if (FrontSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			FrontSideMeshComponent->SetStaticMesh(nullptr);
			FrontSide->bEditableWhenInherited = false;
			FrontSide->UnregisterComponent();
			FrontSide->DestroyComponent(false);
		}

		if (MyObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyObject is nullptr"));
			FrontSideMeshComponent->SetStaticMesh(nullptr);
			FrontSide = nullptr;
		}
		else
		{
			auto x = NewObject<UFleshCubeSideBase>(this, BlueprintFaces.FindRef(FrontSideType));

			if (x == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			FrontSide = x;
			FrontSide->RegisterComponent();
			FrontSide->bEditableWhenInherited = false;

			UStaticMesh* MeshToUse = FrontSide->GetFaceMesh();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from leftside"));
			}

			if (FrontSideMeshComponent == nullptr || FrontSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			FrontSideMeshComponent->SetStaticMesh(FrontSide->GetFaceMesh());
		}

		FrontSide->SetCurrentSideType(FrontSideType);
		PreviousFrontSide = FrontSideType;
	}
}

void AFleshCube::SetupRightSide()
{
	if (RightSideType != PreviousRightSide)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing side on FleshCube"));

		auto MyObject = BlueprintFaces.Find(RightSideType);

		if (RightSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			RightSideMeshComponent->SetStaticMesh(nullptr);
			RightSide->bEditableWhenInherited = false;
			RightSide->UnregisterComponent();
			RightSide->DestroyComponent(false);
		}

		if (MyObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyObject is nullptr"));
			RightSideMeshComponent->SetStaticMesh(nullptr);
			RightSide = nullptr;
		}
		else
		{
			auto x = NewObject<UFleshCubeSideBase>(this, BlueprintFaces.FindRef(RightSideType));

			if (x == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			RightSide = x;
			RightSide->RegisterComponent();
			RightSide->bEditableWhenInherited = false;

			UStaticMesh* MeshToUse = RightSide->GetFaceMesh();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from leftside"));
			}

			if (RightSideMeshComponent == nullptr || RightSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			RightSideMeshComponent->SetStaticMesh(RightSide->GetFaceMesh());
		}

		RightSide->SetCurrentSideType(RightSideType);
		PreviousRightSide = RightSideType;
	}
}

void AFleshCube::SetupBackSide()
{
	if (BackSideType != PreviousBackSide)
	{
		UE_LOG(LogTemp, Warning, TEXT("Changing side on FleshCube"));

		auto MyObject = BlueprintFaces.Find(BackSideType);

		if (BackSide != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cube side is nullptr"));
			BackSideMeshComponent->SetStaticMesh(nullptr);
			BackSide->bEditableWhenInherited = false;
			BackSide->UnregisterComponent();
			BackSide->DestroyComponent(false);
		}

		if (MyObject == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MyObject is nullptr"));
			BackSideMeshComponent->SetStaticMesh(nullptr);
			BackSide = nullptr;
		}
		else
		{
			auto x = NewObject<UFleshCubeSideBase>(this, BlueprintFaces.FindRef(BackSideType));

			if (x == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("x is nullptr"));
				return;
			}

			BackSide = x;
			BackSide->RegisterComponent();
			BackSide->bEditableWhenInherited = false;

			UStaticMesh* MeshToUse = BackSide->GetFaceMesh();

			if (MeshToUse == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("No mesh from leftside"));
			}

			if (BackSideMeshComponent == nullptr || BackSide == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("SideMesh or cubeside is nullptr"));
				return;
			}

			BackSideMeshComponent->SetStaticMesh(BackSide->GetFaceMesh());
		}

		BackSide->SetCurrentSideType(BackSideType);
		PreviousBackSide = BackSideType;
	}
}
