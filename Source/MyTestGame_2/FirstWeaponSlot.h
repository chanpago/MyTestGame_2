// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirstWeaponSlot.generated.h"

/**
 * 
 */

class UItemBase;
class UImage;
class APlayerCharacter;
class UTextBlock;
class UHpExpBar;


UCLASS()
class MYTESTGAME_2_API UFirstWeaponSlot : public UUserWidget
{
	GENERATED_BODY()


public:
	
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* WeaponIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* FirstWeaponName;

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

	void RefreshWeaponSlot();

	void SetHpExpBar(UHpExpBar* InHpExpBar);

protected:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY()
	UHpExpBar* HpExpBar;
};
