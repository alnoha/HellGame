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

		UFleshCubeSideBase* OtherCubeSide = OtherCube->GetCubeSideByCollider(OverlappedComp->GetName());

		if (OtherCubeSide == nullptr)
		{
			return;
		}

		if (OverlappedComp->GetName().Equals(LeftSideBoxCollider->GetName()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("its keft comp"));
			LeftConnectedCube = ConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
		else if (OverlappedComp->GetName().Equals(FrontSideBoxCollider->GetName()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("its frint comp"));
			FrontConnectedCube = ConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
		else if (OverlappedComp->GetName().Equals(RightSideBoxCollider->GetName()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("its right comp"));
			RightConnectedCube = ConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
		else if (OverlappedComp->GetName().Equals(BackSideBoxCollider->GetName()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("its bakc comp"));
			BackConnectedCube = ConnectedCubeInfo(OtherCube, OtherCubeSide);
		}
	}
}

void AFleshCube::OnSideCollisionExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AFleshCube>())
	{
		if (OverlappedComp == LeftSideBoxCollider)
		{
			LeftConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
		}
		else if (OverlappedComp == FrontSideBoxCollider)
		{
			FrontConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
		}
		else if (OverlappedComp == RightSideBoxCollider)
		{
			RightConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
		}
		else if (OverlappedComp == BackSideBoxCollider)
		{
			BackConnectedCube = ConnectedCubeInfo(nullptr, nullptr);
		}
	}
}

void AFleshCube::SendActivationSignal(UFleshCubeSideBase* SendingSide, UFleshCubeSideBase* ReceivingSide, ESideType SendingType)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Received Activation Signal!!"));

	ReceivingSide->ReceivedActivationSignal(SendingSide, SendingType);
	
	/*ReceivingSide->CallFunctionByNameWithArguments(TEXT("ReceivedActivationSignal"), SendingSide, SendingType);*/
	
	//ExecReceivedActivationSignal(SendingSide, SendingType);
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
	LeftSideBoxCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	LeftSideBoxCollider->SetRelativeLocation(FVector(-130.0f, 0.0f, 0.0f));

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
	FrontSideBoxCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	FrontSideBoxCollider->SetRelativeLocation(FVector(-130.0f, 0.0f, 0.0f));

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
	RightSideBoxCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RightSideBoxCollider->SetRelativeLocation(FVector(-130.0f, 0.0f, 0.0f));

	BackSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Back Side Mesh");
	BackSideMeshComponent->AttachToComponent(BaseMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BackSideMeshComponent->bEditableWhenInherited = true;
	BackSideMeshComponent->RegisterComponent();
	BackSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	BackSideBoxCollider = CreateDefaultSubobject<UBoxComponent>("Back Box collider");
	BackSideBoxCollider->bEditableWhenInherited = true;
	BackSideBoxCollider->AttachToComponent(BackSideMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BackSideBoxCollider->RegisterComponent();
	BackSideBoxCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BackSideBoxCollider->SetRelativeLocation(FVector(-130.0f, 0.0f, 0.0f));

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

	if (GetWorld()->LineTraceSingleByChannel(GroundCheckHitResult, this->GetActorLocation(), this->GetActorLocation() + (FVector::DownVector * 10000), ECC_Visibility))
	{
		DrawDebugLine(GetWorld(), this->GetActorLocation(), this->GetActorLocation() + (FVector::DownVector * 10000), FColor::Magenta);
		if (FVector::Distance(this->GetActorLocation(), GroundCheckHitResult.ImpactPoint) < 64.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Found ground"));
			if (LeftConnectedCube.ConnectedCube != nullptr)
			{
				if (LeftConnectedCube.ConnectedFace == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("left = null"));

				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("left != null"));
					LeftConnectedCube.ConnectedCube->SendActivationSignal(LeftSide, LeftConnectedCube.ConnectedFace, LeftSideType);
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
					FrontConnectedCube.ConnectedCube->SendActivationSignal(FrontSide, FrontConnectedCube.ConnectedFace, FrontSideType);
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
					RightConnectedCube.ConnectedCube->SendActivationSignal(RightSide, RightConnectedCube.ConnectedFace, RightSideType);
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
					BackConnectedCube.ConnectedCube->SendActivationSignal(BackSide, BackConnectedCube.ConnectedFace, BackSideType);
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
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("its left but its null"));
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
		if (BackSide == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("its back but its null"));
		}
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
	SetupSide(LeftSideMeshComponent, LeftSideType, PreviousLeftSide, LeftSide);
	SetupSide(FrontSideMeshComponent, FrontSideType, PreviousFrontSide, FrontSide);
	SetupSide(RightSideMeshComponent, RightSideType, PreviousRightSide, RightSide);
	SetupSide(BackSideMeshComponent, BackSideType, PreviousBackSide, BackSide);
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
