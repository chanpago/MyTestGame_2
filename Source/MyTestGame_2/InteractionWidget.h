// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class APlayerCharacter;
struct FInteractableData;
class UTextBlock;
class UProgressBar;

UCLASS()
class MYTESTGAME_2_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(VisibleAnyWhere, Category = "Interaction Widget | Player Reference")
	APlayerCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData) const;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;


protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;



	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();



};
