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
	FStringClassReference CrosshairWidgetClassRef(TEXT("/Game/FirstPersonCPP/UserInterface/WBP_HUD.WBP_HUD_C"));
	if (UClass* Widget = CrosshairWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
		CrosshairWidget->AddToViewport();
	}
	InitCrosshairTextures();
}

void AHellGameHUD::InitCrosshairTextures()
{
	//TODO: Change this to load from a directory instead of hardcoded Path
	CrosshairTextureMap.Add(ECrosshairTypes::DEFAULT, Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/FirstPerson/Textures/UI/Crosshair/T_Cross_01.T_Cross_01"))));
	CrosshairTextureMap.Add(ECrosshairTypes::PICKUP, Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/FirstPerson/Textures/UI/Crosshair/T_Cross_02.T_Cross_02"))));
	CrosshairTextureMap.Add(ECrosshairTypes::ROTATE, Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/FirstPerson/Textures/UI/Crosshair/T_Cross_03.T_Cross_03"))));
	CrosshairTextureMap.Add(ECrosshairTypes::DOOR, Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/FirstPerson/Textures/UI/Crosshair/T_Cross_04.T_Cross_04"))));
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
