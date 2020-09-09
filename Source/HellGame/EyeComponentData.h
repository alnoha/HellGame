#pragma once
#include "EyeComponentData.generated.h"

USTRUCT(Blueprintable)
struct FEyeComponentData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale = FVector(0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPersistent = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 11.0f;
};