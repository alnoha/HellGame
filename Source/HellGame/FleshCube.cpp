// Fill out your copyright notice in the Description page of Project Settings.


#include "FleshCube.h"

// Sets default values
AFleshCube::AFleshCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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
	LeftSideMeshComponent->AttachTo(BaseMesh);
	LeftSideMeshComponent->RegisterComponent();
	LeftSideMeshComponent->bEditableWhenInherited = true;
	LeftSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	LeftSideMeshComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	FrontSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Front Side Mesh");
	FrontSideMeshComponent->AttachTo(BaseMesh);
	FrontSideMeshComponent->RegisterComponent();
	FrontSideMeshComponent->bEditableWhenInherited = true;
	FrontSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	FrontSideMeshComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	RightSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Right Side Mesh");
	RightSideMeshComponent->AttachTo(BaseMesh);
	RightSideMeshComponent->RegisterComponent();
	RightSideMeshComponent->bEditableWhenInherited = true;
	RightSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	RightSideMeshComponent->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	BackSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Back Side Mesh");
	BackSideMeshComponent->AttachTo(BaseMesh);
	BackSideMeshComponent->bEditableWhenInherited = true;
	BackSideMeshComponent->RegisterComponent();
	BackSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	TopSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Top Side Mesh");
	TopSideMeshComponent->AttachTo(BaseMesh);
	TopSideMeshComponent->bEditableWhenInherited = true;
	TopSideMeshComponent->RegisterComponent();
	TopSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	TopSideMeshComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	BottomSideMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Bottom Side Mesh");
	BottomSideMeshComponent->AttachTo(BaseMesh);
	BottomSideMeshComponent->bEditableWhenInherited = true;
	BottomSideMeshComponent->RegisterComponent();
	BottomSideMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	BottomSideMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AFleshCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFleshCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFleshCube::OnConstruction(const FTransform& Transform)
{
	SetupSides();

	Super::OnConstruction(Transform);
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
