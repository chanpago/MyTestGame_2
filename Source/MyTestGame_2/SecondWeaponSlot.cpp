// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "SecondWeaponSlot.h"
#include "ItemBase.h"
#include "PlayerCharacter.h"
#include "MyTestWeapon.h"

//engine
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void USecondWeaponSlot::SetItemReference(UItemBase* ItemIn)
{

	ItemReference = ItemIn;

	UE_LOG(LogTemp, Warning, TEXT("SetItemReference Set"));

}

void USecondWeaponSlot::RefreshInventory()
{

}

void USecondWeaponSlot::SetImageIcon() const
{
	PlayerCharacter->IsChange = false;
	UE_LOG(LogTemp, Warning, TEXT("SetImageIcon set"));
	WeaponIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);


}

void USecondWeaponSlot::NativeOnInitialized()
{

	Super::NativeOnInitialized();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());


	if (PlayerCharacter)
	{
		ItemReference = PlayerCharacter->GetWeaponReference();
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter set"));
		if (ItemReference)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemReference set"));
			SetImageIcon();

		}
	}

	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/_My/Image/Itemempty.Itemempty'"));
	WeaponIcon->SetBrushFromTexture(Texture);
}

void USecondWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void USecondWeaponSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (PlayerCharacter->GetWeaponReference() && PlayerCharacter->IsChange)
	{
		ItemReference = PlayerCharacter->GetWeaponReference();
		this->SetImageIcon();
		SecondWeaponName->SetText(ItemReference->TextData.Name);
		//UE_LOG(LogTemp, Warning, TEXT("NativeConstruct: WeaponIcon set"));
		//WeaponIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
	}
}

FReply USecondWeaponSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (ItemReference->bIsUsed)
		{
			// UI 요소 업데이트
			SecondWeaponName->SetText(FText::FromString(TEXT("")));
			WeaponIcon->SetBrush(FSlateBrush());

			//캐릭터에 특정지점 무기 삭제
			PlayerCharacter->SecondWeapon->Destroy();
			PlayerCharacter->SecondWeapon = nullptr;

			PlayerCharacter->SecondSlot = false;
			ItemReference->bIsUsed = false;
			//PlayerCharacter->CurrentWeapon = nullptr;
		}

	}
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/_My/Image/Itemempty.Itemempty'"));
	WeaponIcon->SetBrushFromTexture(Texture);
	return Reply.Unhandled();
}

