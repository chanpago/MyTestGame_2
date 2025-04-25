// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "InventoryItemSlot.h"
#include "ItemBase.h"
#include "InventoryTooltip.h"
#include "DragItemVisual.h"
#include "InventoryComponent.h"
#include "ItemDragDropOperation.h"
#include "MyTestWeapon.h"
#include "FirstWeaponSlot.h"

//engine
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UInventoryItemSlot::NativeOnInitialized()
{
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	IsUsed = false;

	if (ToolTipClass)
	{
		UInventoryTooltip* ToolTip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}

	//FirstWeaponSlot = Cast<UFirstWeaponSlot>();
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference->ItemQuality)
		{
		case EItemQuality::Shoddy:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::Common: 
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::Quality: 
			ItemBorder->SetBrushColor(FLinearColor(0.5f, 0.0f, 0.7f));
			break;
		case EItemQuality::Masterwork: 
			ItemBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f));
			break;
		case EItemQuality::Grandmaster:
			ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.51f, 0.169f));
			break;
		default: ;
		}

		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
			UE_LOG(LogTemp, Warning, TEXT("stackable set"));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
			UE_LOG(LogTemp, Warning, TEXT("stackable not set"));
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (ItemReference->ItemType == EItemType::Consumable)
		{
			//ItemReference->Quantity--;
			//isused가 true이면 tick에서 consumable처리를 하여 갯수가 하나 줄어듦
			IsUsed = true;
			ItemReference->Use(PlayerCharacter);
		}
		else if (ItemReference->ItemType == EItemType::Weapon)
		{
			//메인에 무기가 있고 서브에 없는 조건
			
			UE_LOG(LogTemp, Warning, TEXT("RightMouseButton set"));


			/*
			PlayerCharacter->SetWeaponReference(ItemReference);
			PlayerCharacter->IsChange = true;

			//무기 장착
			PlayerCharacter->SpawnDefaultInventory();
			*/
			if (!ItemReference->bIsUsed)
			{
				//UE_LOG(LogTemp, Warning, TEXT("!ItemReference->bIsUsed set"));
				if (PlayerCharacter->FirstSlot == false)
				{
					//UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter->FirstSlot == false set"));
					PlayerCharacter->SetWeaponReference(ItemReference);
					PlayerCharacter->IsChange = true;

					//무기 장착
					PlayerCharacter->SpawnDefaultInventory();

					PlayerCharacter->FirstSlot = true;

					//UE_LOG(LogTemp, Warning, TEXT("1"));
					ItemReference->bIsUsed = true;
				}
				else if (PlayerCharacter->FirstSlot == true && PlayerCharacter->SecondSlot == false)
				{
					PlayerCharacter->SetWeaponReference(ItemReference);
					PlayerCharacter->IsChange = true;

					//무기 장착
					PlayerCharacter->SpawnDefaultInventory();

					PlayerCharacter->SecondSlot = true;
					//UE_LOG(LogTemp, Warning, TEXT("2"));
					ItemReference->bIsUsed = true;
				}
				else
				{

				}
			}
		}	
	}
	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryItemSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (IsUsed) 
	{
		ItemReference->Quantity--;
		UInventoryComponent* component = ItemReference->OwningInventory;
		float getweight = component->GetInventoryTotalWeight();
		getweight -= ItemReference->GetItemSingleWeight();
		component->SetInventoryTotalWeight(getweight);
		
		IsUsed = false;
		ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UE_LOG(LogTemp, Warning, TEXT("Quantity down!"));
		if (ItemReference->Quantity <= 0)
		{
			//ItemReference = nullptr;
			
			//UInventoryComponent* component = ItemReference->OwningInventory;
			component->RemoveSingleInstanceOfItem(ItemReference);
			//ItemReference->OwningInventory->RemoveSingleInstanceOfItem(ItemReference);
			RemoveFromParent();
			ConditionalBeginDestroy();
			UE_LOG(LogTemp, Warning, TEXT("Item removed!"));
		}
	}


}

/*
void UInventoryItemSlot::NativeRightMouseButton(APlayerCharacter* PlayerCharacter, const FPointerEvent& InMouseEvent)
{
	if (ItemReference)
	{
		if (ItemReference->ItemType == EItemType::Consumable)
		{
			ItemReference->Use(PlayerCharacter);
		}
	}
}
*/