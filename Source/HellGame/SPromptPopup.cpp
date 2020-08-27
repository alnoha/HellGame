// Fill out your copyright notice in the Description page of Project Settings.


#include "SPromptPopup.h"
//#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "PromptPickup"
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPromptPopup::Construct(const FArguments& InArgs)
{
	const FMargin TextPosition = FMargin(500.0f, 250.0f);
	const FText PromptText = LOCTEXT("PickupPrompt", "Press ? to pickup");

	FSlateFontInfo PromptTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	PromptTextStyle.Size = 40.0f;

	ChildSlot
		[
			// Populate the widget
			SNew(SOverlay)
			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(TextPosition)
		[
			SNew(STextBlock)
			.Font(PromptTextStyle)
		.Text(PromptText)
		.ColorAndOpacity(FColor::Blue)
		.Justification(ETextJustify::Center)
		]
		];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
#undef LOCTEXT_NAMESPACE