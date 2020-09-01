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
