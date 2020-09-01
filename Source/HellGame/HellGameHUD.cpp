// Copyright Epic Games, Inc. All Rights Reserved.

#include "HellGameHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

AHellGameHUD::AHellGameHUD()
{

}

void AHellGameHUD::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	InitCrosshairTextures();
	FStringClassReference CrosshairWidgetClassRef(TEXT("/Game/FirstPersonCPP/UserInterface/WBP_HUD.WBP_HUD_C"));
	if (UClass* Widget = CrosshairWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
		CrosshairWidget->AddToViewport();
	}
}

void AHellGameHUD::InitCrosshairTextures()
{
	TArray<UObject*> Assets;
	FindOrLoadAssetsByPath(FString("/Game/Textures/UI/Crosshair/"), Assets, EngineUtils::ATL_Regular);
	for (auto Asset : Assets)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Asset->GetName());
		UTexture2D* Texture = Cast<UTexture2D>(Asset);
		if (Texture)
		{
			CrosshairTextures.Add(Texture);
		}
	}
}

void AHellGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AHellGameHUD::ShowWidget(TSubclassOf<UUserWidget> Widget)
{
	if (Widget != nullptr)
	{
		PromptWidget = CreateWidget(GetWorld(), Widget);
		PromptWidget->AddToViewport();
	}
}

void AHellGameHUD::HideWidget()
{
	if (PromptWidget != nullptr)
	{
		PromptWidget->RemoveFromViewport();
		PromptWidget = nullptr;
	}
}

void AHellGameHUD::UpdateCrosshair(UTexture2D* Texture)
{
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(CrosshairWidget->GetRootWidget());
	// Crosshair image got index 0
	UImage* Image = Cast<UImage>(CanvasPanel->GetChildAt(0));
	Image->SetBrushFromTexture(Texture);
}
