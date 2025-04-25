// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataStructs.h"
#include "PlayerCharacter.h"
#include "ItemBase.generated.h"


class UInventoryComponent;

UCLASS()
class MYTESTGAME_2_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY()
	UInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;

	bool bIsCopy;
	bool bIsPickup;
	bool bIsUsed;

	// 아이템의 크기 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FVector ItemScale = FVector(2.0f, 2.0f, 2.0f); // 기본값 설정

	FTimerHandle TimerHandle;  // TimerHandle 선언

	//APlayerCharacter* PlayerCharacter;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	UItemBase();

	void ResetItemFlags();

	void Equip();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(APlayerCharacter* PlayerCharacter);

	

	
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FVector GetItemScale() const { return ItemScale; }
	
protected:


	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	
	

	//=========================================================================
	// FUCTIONS
	//=========================================================================


	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}





	
};
