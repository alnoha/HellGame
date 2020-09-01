// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "EngineUtils.h"
#include "HellGameHUD.generated.h"

UCLASS()
class AHellGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AHellGameHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	void ShowWidget(TSubclassOf<UUserWidget> Widget);
	void HideWidget();

	void UpdateCrosshair(UTexture2D* Texture);
	TArray<UTexture2D*> CrosshairTextures;

protected:
	virtual void BeginPlay();
	UUserWidget* PromptWidget;
	UUserWidget* CrosshairWidget;

private:
	void InitCrosshairTextures();
};

