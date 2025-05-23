// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "InventoryTooltip.h"
#include "InventoryItemSlot.h"
#include "ItemBase.h"

//engine
#include "Components/TextBlock.h"


void UInventoryTooltip::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	const FString WeightInfo = { "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

	StackWeight->SetText(FText::FromString(WeightInfo));
}

void UInventoryTooltip::NativeConstruct()
{

	Super::NativeConstruct();

	if (const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference())
	{
		switch (ItemBeingHovered->ItemQuality)
		{
		case EItemQuality::Shoddy:
			ItemType->SetColorAndOpacity(FLinearColor::Gray);
			break;
		case EItemQuality::Common:
			ItemType->SetColorAndOpacity(FLinearColor::White);
			break;
		case EItemQuality::Quality:
			ItemType->SetColorAndOpacity(FLinearColor(0.0f, 0.51f, 0.169f));
			break;
		case EItemQuality::Masterwork:
			ItemType->SetColorAndOpacity(FLinearColor(0.0f,0.4f,0.75f));
			break;
		case EItemQuality::Grandmaster:
			ItemType->SetColorAndOpacity(FLinearColor(1.0f,0.45f,0.0f));
			break;
		}



		switch (ItemBeingHovered->ItemType)
		{
		case EItemType::Armor:

			break;
		case EItemType::Weapon:
			ItemType->SetText(FText::FromString("Weapon"));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case EItemType::Shield:

			break;
		case EItemType::Spell:

			break;
		case EItemType::Consumable:
			ItemType->SetText(FText::FromString("Consumable"));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			ArmorRating->SetVisibility(ESlateVisibility::Collapsed);

			break;
		case EItemType::Quest:

			break;
		case EItemType::Mundane:
			ItemType->SetText(FText::FromString("Mundane Item"));
			DamageValue->SetVisibility(ESlateVisibility::Collapsed);
			ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
			UsageText->SetVisibility(ESlateVisibility::Collapsed);

			break;
		default:;
		}

		ItemName->SetText(ItemBeingHovered->TextData.Name);
		DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
		ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));
		UsageText->SetText(ItemBeingHovered->TextData.UsageText);
		ItemDescription->SetText(ItemBeingHovered->TextData.Description);

		const FString WeightInfo = { "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

		StackWeight->SetText(FText::FromString(WeightInfo));


		if (ItemBeingHovered->NumericData.bIsStackable)
		{
			const FString StackInfo = { "Max stack size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize) };

			MaxStackSize->SetText(FText::FromString(StackInfo));
		}
		else
		{
			MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
