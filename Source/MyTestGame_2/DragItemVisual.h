// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

/**
 * 
 */

class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class MYTESTGAME_2_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()
	

public:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* ItemQuantity;

};
