// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SecondWeaponSlot.generated.h"

/**
 * 
 */

class UItemBase;
class UImage;
class APlayerCharacter;
class UTextBlock;

UCLASS()
class MYTESTGAME_2_API USecondWeaponSlot : public UUserWidget
{
	GENERATED_BODY()
	
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* WeaponIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* SecondWeaponName;

	UPROPERTY()
	UItemBase* ItemReference;

	APlayerCharacter* PlayerCharacter;

	//=========================================================================
	// FUCTIONS
	//=========================================================================


	void SetItemReference(UItemBase* ItemIn);

	UFUNCTION()
	void RefreshInventory();

	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; };

	FORCEINLINE void SetImageIcon(UImage* Icon) { WeaponIcon = Icon; };

	void SetImageIcon() const;

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;



protected:



	//=========================================================================
	// FUCTIONS
	//=========================================================================
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
