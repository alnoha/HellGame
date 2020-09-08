#pragma once
#include "FleshCubeSideBase.h"
#include "SideTypes.h"
#include "SideComponentData.generated.h"

USTRUCT(Blueprintable)
struct HELLGAME_API FSideComponentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UFleshCubeSideBase> Blueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ESideType> FaceMatches;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeActivatedByPoop;
};