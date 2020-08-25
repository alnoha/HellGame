// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SideComponent.generated.h"


UENUM()
enum class ECubeSide
{
	None,
	Eye,
	Mouth
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USideComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HELLGAME_API ISideComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
