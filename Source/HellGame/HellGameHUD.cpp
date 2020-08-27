// Copyright Epic Games, Inc. All Rights Reserved.

#include "HellGameHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "SPromptPopup.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

AHellGameHUD::AHellGameHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AHellGameHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X),
		(Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
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
