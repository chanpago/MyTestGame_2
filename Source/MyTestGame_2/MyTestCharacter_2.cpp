// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestCharacter_2.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "HUD_InGame.h"
#include "HpExpBar.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "MyBasicCharacter_2.h"
#include "Components/TextBlock.h" 
#include "PlayerHitDamage.h"
#include "GameInstance_Player.h"
#include "MyTestWeapon.h"

AMyTestCharacter_2::AMyTestCharacter_2() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject< UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;


	SetupStimulus();

	HPEXPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPEXPValue"));

	if (HPEXPWidgetComponent)
	{
		
		HPEXPWidgetComponent->SetupAttachment(RootComponent);
		HPEXPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		
		static ConstructorHelpers::FClassFinder<UUserWidget>HPEXPWidgetClass(TEXT("/Game/_My/UI/HpExpBar.HpExpBar_C"));
		if (HPEXPWidgetClass.Succeeded())
		{
			UE_LOG(LogTemp, Warning, TEXT("HPEXPWIDGETCLASS(HUD_InGame) loaded Successfully"));
			//UE_LOG(LogTemp, Warning, TEXT("WC loaded Successfully"));
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, __FUNCTION__);
			HPEXPWidgetComponent->SetWidgetClass(HPEXPWidgetClass.Class);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HPEXPWIDGETCLASS not loaded Successfully"));
		}
	}

}


void AMyTestCharacter_2::BeginPlay()
{
	Super::BeginPlay();

	/*
	UGameInstance_Player* GameInstance = Cast<UGameInstance_Player>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetPlayerReference(this);
	}

	GameInstance->LoadPlayerData();
	*/
}

// Called every frame
void AMyTestCharacter_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto const HudHp = Cast<UHpExpBar>(HPEXPWidgetComponent->GetUserWidgetObject());
	MyHPnum = (MyHealth / MyMaxHealth * 100) * 0.01f;
	MyEXPnum = (MyEXP / MyMaxEXP * 100) * 0.01f;
	if (HudHp)
	{
		HudHp->AddToViewport();
		//UE_LOG(LogTemp, Warning, TEXT("HudHp loaded Successfully"));
		HudHp->SetHPBarValuePercent(MyHPnum);

	}
	else if (!HudHp) {
		//UE_LOG(LogTemp, Warning, TEXT("UW not loaded Successfully"));
	}


	if (MyEXP >= MyMaxEXP) {
		MyLevel++;
		MyEXP -=MyMaxEXP;
		MyMaxEXP *= 1.5;
		MyHealth = 100;
		MyDamage += MyLevel;
	}
	MyEXPnum = (MyEXP / MyMaxEXP * 100) * 0.01f;
	

	if (bRightMouseClicked)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			float MouseX, MouseY;
			PlayerController->GetInputMouseDelta(MouseX, MouseY);
			AddControllerYawInput(MouseX * TurnRate * GetWorld()->GetDeltaSeconds());
			AddControllerPitchInput(MouseY * LookUpRate * GetWorld()->GetDeltaSeconds());
		}
	}

	/*
	UGameInstance_Player* GameInstance = Cast<UGameInstance_Player>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetPlayerReference(this);
	}

	GameInstance->SavePlayerData();
	*/
}

void AMyTestCharacter_2::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
}

void AMyTestCharacter_2::AddEXP(float EXP)
{
	MyEXP += EXP;
}

void AMyTestCharacter_2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyTestCharacter_2::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyTestCharacter_2::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyTestCharacter_2::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyTestCharacter_2::StopJump);

	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMyTestCharacter_2::AttackMelee);
	PlayerInputComponent->BindAction("Combo1", IE_Released, this, &AMyTestCharacter_2::ComboMelee1);
	PlayerInputComponent->BindAction("Combo2", IE_Released, this, &AMyTestCharacter_2::ComboMelee2);
	PlayerInputComponent->BindAction("Evade", IE_Released, this, &AMyTestCharacter_2::Evade);
	PlayerInputComponent->BindAction("Slash", IE_Released, this, &AMyTestCharacter_2::Slash);

	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AMyTestCharacter_2::OnRightMouseClick);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &AMyTestCharacter_2::OnRightMouseRelease);
	
	PlayerInputComponent->BindAxis("Turn", this, &AMyTestCharacter_2::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyTestCharacter_2::LookUp);

	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AMyTestCharacter_2::OpenInventory);
}

void AMyTestCharacter_2::Turn(float Value)
{
	// 마우스 X 이동에 대한 회전 속도를 조절
	AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyTestCharacter_2::LookUp(float Value)
{
	// 마우스 Y 이동에 대한 회전 속도를 조절
	AddControllerPitchInput(Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyTestCharacter_2::OnRightMouseClick()
{
	bRightMouseClicked = true;
}

void AMyTestCharacter_2::OnRightMouseRelease()
{
	bRightMouseClicked = false;
}

void AMyTestCharacter_2::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && IsDuringAttack==false)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMyTestCharacter_2::MoveRight(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && IsDuringAttack == false)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

// 인벤토리를 여는 함수
void AMyTestCharacter_2::OpenInventory()
{
	/*
	if (InventoryItem->IsVisible())
		InventoryItem->VisibleInventory(ESlateVisibility::Collapsed);
	else
		InventoryItem->VisibleInventory(ESlateVisibility::Visible);
	*/

}

void AMyTestCharacter_2::StartJump()
{
	bPressedJump = true;
}

void AMyTestCharacter_2::StopJump()
{
	bPressedJump = false;
}

void AMyTestCharacter_2::SetupStimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

