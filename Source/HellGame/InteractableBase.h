// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "HellGameHUD.h"
#include "InteractableBase.generated.h"

UCLASS()
class HELLGAME_API AInteractableBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AHellGameHUD* HUD;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnBeginFocus();
	virtual void OnBeginFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
		void OnEndFocus();
	virtual void OnEndFocus_Implementation();

	UPROPERTY(BlueprintReadWrite, Category = "Interactable")
		bool bIsFocused;

	UPROPERTY(EditAnywhere, Category = "Interactable")
		TSubclassOf<UUserWidget> PromptWidget;
	//Sync
};
