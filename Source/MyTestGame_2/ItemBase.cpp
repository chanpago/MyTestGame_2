// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "ItemBase.h"
#include "TimerManager.h"
#include "InventoryComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{

}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
	bIsUsed = false;
}

void UItemBase::Equip()
{

}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}



void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != this->Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, this->NumericData.bIsStackable ? this->NumericData.MaxStackSize : 1);
		if (this->OwningInventory)
		{
			if(this->Quantity <= 0)
			{
				this->OwningInventory ->RemoveSingleInstanceOfItem(this);
			}
		}
	}
}

void UItemBase::Use(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->IncreaseHealth(PlayerCharacter);
}

