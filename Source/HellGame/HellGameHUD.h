// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HellGameHUD.generated.h"

UCLASS()
class AHellGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AHellGameHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	void ShowPrompt();
	void HidePrompt();

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

protected:
	TSharedPtr<class SPromptPopup> PromptWidget;
	TSharedPtr<class SWidget> PromptWidgetContainer;
};

