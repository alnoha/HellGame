// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "EngineUtils.h"
#include "HellGameHUD.generated.h"

UENUM()
enum class ECrosshairTypes
{
	DEFAULT,
	PICKUP,
	ROTATE,
	DOOR
};

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

<<<<<<< HEAD
private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
=======
	void UpdateCrosshair(UTexture2D* Texture);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ECrosshairTypes, UTexture2D*> CrosshairTextureMap;
>>>>>>> 11309d81c5a51b42a286b528578459c84d47d5c8

protected:
	virtual void BeginPlay();
	UUserWidget* PromptWidget;
	UUserWidget* CrosshairWidget;

private:
	void InitCrosshairTextures();
};

