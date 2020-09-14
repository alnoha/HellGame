#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SideTypes.h"
#include "EyeComponentData.h"
#include "FleshCubeSideBase.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HELLGAME_API UFleshCubeSideBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFleshCubeSideBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (ToolTip = "The face this side should use."))
	USkeletalMesh* FaceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (ToolTip = "The material this side should use."))
	UMaterialInstance* FaceMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (ToolTip = "The animation blueprint this side should use."))
	TSubclassOf<UAnimInstance> FaceAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Side Type", meta = (ToolTip = "The current sidetype."))
	ESideType CurrentSideType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceivedActivationSignal(UFleshCubeSideBase* SendingSide, ESideType SendingType, USkeletalMeshComponent* MeshComponent, FTransform SideTransform);
	virtual void ReceivedActivationSignal_Implementation(UFleshCubeSideBase* SendingSide, ESideType SendingType, USkeletalMeshComponent* MeshComponent, FTransform SideTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceivedStopSignal();
	virtual void ReceivedStopSignal_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Activation signals")
	void ReceiveRemoteActivationSignal(FTransform SideTransform);
	virtual void ReceiveRemoteActivationSignal_Implementation(FTransform SideTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Init Side")
	void Initialize_Side(FEyeComponentData EyeData, FVector SnotScale);
	virtual void Initialize_Side_Implementation(FEyeComponentData EyeData, FVector SnotScale);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	USkeletalMesh* GetFaceMesh();
	UMaterialInstance* GetFaceMaterial();
	TSubclassOf<UAnimInstance> GetAnimationInstance();

	void SetCurrentSideType(ESideType NewSideType);
	ESideType GetCurrentSideType();
};
