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
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void AHellGameHUD::ShowPrompt()
{
	if (GEngine && GEngine->GameViewport)
	{
		PromptWidget = SNew(SPromptPopup).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(PromptWidgetContainer, SWeakWidget).PossiblyNullContent(PromptWidget.ToSharedRef()));
	}
}

void AHellGameHUD::HidePrompt()
{
	if (GEngine && GEngine->GameViewport && PromptWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(PromptWidgetContainer.ToSharedRef());
	}
}
