//game
#include "FirstWeaponSlot.h"
#include "ItemBase.h"
#include "PlayerCharacter.h"
#include "MyTestWeapon.h"
#include "HpExpBar.h"

//engine
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UFirstWeaponSlot::SetItemReference(UItemBase* ItemIn)
{
	ItemReference = ItemIn;
	//UE_LOG(LogTemp, Warning, TEXT("SetItemReference Set"));
}

void UFirstWeaponSlot::RefreshInventory()
{
}

void UFirstWeaponSlot::SetImageIcon() const
{
	PlayerCharacter->IsChange = false;
	WeaponIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
}

void UFirstWeaponSlot::NativeOnInitialized()
{

	Super::NativeOnInitialized();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());

	if (PlayerCharacter)
	{
		//ItemReference = PlayerCharacter->GetWeaponReference();
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter set"));
		PlayerCharacter->FirstOnWeaponChanged.AddUObject(this, &UFirstWeaponSlot::RefreshWeaponSlot);	
	}

	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/_My/Image/Itemempty.Itemempty'"));
	WeaponIcon->SetBrushFromTexture(Texture);
}

void UFirstWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFirstWeaponSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UFirstWeaponSlot::RefreshWeaponSlot()
{
	ItemReference = PlayerCharacter->GetWeaponReference();
	SetImageIcon();
	FirstWeaponName->SetText(ItemReference->TextData.Name);

}

void UFirstWeaponSlot::SetHpExpBar(UHpExpBar* InHpExpBar)
{
	HpExpBar = InHpExpBar;
}

FReply UFirstWeaponSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (ItemReference->bIsUsed)
		{
			// UI 요소 업데이트
			FirstWeaponName->SetText(FText::FromString(TEXT("")));
			WeaponIcon->SetBrush(FSlateBrush());

			//캐릭터에 특정지점 무기 삭제
			PlayerCharacter->FirstWeapon->Destroy();
			PlayerCharacter->FirstWeapon = nullptr;
			PlayerCharacter->FirstSlot = false;
			ItemReference->bIsUsed = false;
			PlayerCharacter->CurrentWeapon = nullptr;
		}
		
	}
	UTexture2D* Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/_My/Image/Itemempty.Itemempty'"));
	WeaponIcon->SetBrushFromTexture(Texture);
	return Reply.Unhandled();
}

