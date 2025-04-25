// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "PlayerCharacter.h"
#include "MainHUD.h"
#include "InventoryComponent.h"
#include "HpExpBar.h"
#include "Pickup.h"
#include "MyTestWeapon.h"
#include "ItemBase.h"
#include "MyBotCharacter_2.h"
#include "MyBotCharacter_3.h"
#include "FirstWeaponSlot.h"
#include "MySlashAura.h"
#include "BossSill2_Actor.h"
#include "PlayerHitDamage.h"

//engine
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Components/BillboardComponent.h"
#include "GameInstance_Player.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

ACharacter* LockedOnTarget = nullptr;
bool bIsLockedOn = false;
USphereComponent* CollisionSphere = nullptr;
FTimerHandle LockOnFocusTimer;

// 락온된 캐릭터에 흰색 점을 표시하기 위한 위젯 컴포넌트 포인터 추가
UBillboardComponent* LockOnIndicator = nullptr;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// 카메라 충돌 설정 비활성화
	CameraBoom->bDoCollisionTest = false; // 카메라 충돌 테스트 비활성화

	FollowCamera = CreateDefaultSubobject< UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;


	BaseEyeHeight = 74.0f;


	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);

	FirstWeaponInventory = CreateDefaultSubobject<UFirstWeaponSlot>(TEXT("FirstWeaponInventory"));

	MyHealth = 50;

	MaxStamina = 100;

	NowStamina = MaxStamina;

	IsChange = false;

	FirstSlot = false;

	SecondSlot = false;

	
	// 캡슐 컴포넌트의 충돌 설정
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// 루트 컴포넌트가 설정되었는지 확인
	RootComponent = GetCapsuleComponent();

	// 메쉬의 물리 시뮬레이션 비활성화
	GetMesh()->SetSimulatePhysics(false);

	
}



// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &APlayerCharacter::OnRightMouseClick);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &APlayerCharacter::OnRightMouseRelease);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAction("Slash", IE_Released, this, &APlayerCharacter::Slash);
	PlayerInputComponent->BindAction("LockOn", IE_Released, this, &APlayerCharacter::LockOn);
	PlayerInputComponent->BindAction("Roll", IE_Released, this, &APlayerCharacter::Roll);
	PlayerInputComponent->BindAction("AOW", IE_Released, this, &APlayerCharacter::AOW);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::BeginInteract);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::EndInteract);


	PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &APlayerCharacter::ToggleMenu);


	PlayerInputComponent->BindAction("AttackMelee", IE_Pressed, this, &APlayerCharacter::AttackMelee);
	PlayerInputComponent->BindAction("ComboMelee1", IE_Pressed, this, &APlayerCharacter::ComboMelee1);
	PlayerInputComponent->BindAction("ComboMelee2", IE_Pressed, this, &APlayerCharacter::ComboMelee2);

	PlayerInputComponent->BindAction("BackEvade", IE_Pressed, this, &APlayerCharacter::BackEvade);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::Block);
}



// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());


	// 현재 맵 이름 가져오기
	FString CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UE_LOG(LogTemp, Warning, TEXT("Current Map Name: %s"), *CurrentMapName);

	// 맵 이름이 "Demonstration"이 아닌 경우에만 Load() 호출
	if (CurrentMapName == "Demonstration")
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentMapName == Demonstration "));
	}
	else
	{
		// 플레이어 정보 로드
		Load();
	}


	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &APlayerCharacter::OnMontageNotifyBegin);
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &APlayerCharacter::OnMontageNotifyEnd);
	}
	
}

void APlayerCharacter::SetCannotDamaged(bool value){CannotDamaged = value;}

bool APlayerCharacter::GetCannotDamaged(){return CannotDamaged;}

int APlayerCharacter::GetComboAttackNum()
{
	return ComboAttack_Num;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	MyHPnum = (MyHealth / MyMaxHealth * 100) * 0.01f;
	MyEXPnum = (MyEXP / MyMaxEXP * 100) * 0.01f;

	
	
	/*
	if (MyEXP >= MyMaxEXP) {
		MyLevel++;
		MyEXP -= MyMaxEXP;
		MyMaxEXP *= 1.5;
		MyHealth = 100;
		MyDamage += MyLevel;
	}
	*/

	if (NowStamina < MaxStamina) {
		NowStamina+=0.1;
	}


	// 락온 상태에서 디버그 스피어 그리기
	if (bIsLockedOn && CollisionSphere)
	{
		//DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionSphere->GetScaledSphereRadius(), 32, FColor::Green, false, -1.0f, 0, 2.0f);
	}

	// 락온 상태에서 방향 고정
	if (bIsLockedOn && LockedOnTarget)
	{
		if (!IsRolling) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "FocusOn!");
			FocusOnTarget(LockedOnTarget);
		}
	}
	

}

void APlayerCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = DefaultInventoryClass.Num();
	//UE_LOG(LogTemp, Warning, TEXT("SpawnDefaultInventory set1!"));
	FName st = GetWeaponReference()->ID;
	FString NameString = st.ToString();
	int32 Weaponnum = FCString::Atoi(*NameString);
	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Weapon number: %d !!"), Weaponnum);
		if (DefaultInventoryClass[i] && i==Weaponnum)
		{
			FActorSpawnParameters SpawnInfo;
			UWorld* WRLD = GetWorld();
			AMyTestWeapon* NewWeapon = WRLD->SpawnActor<AMyTestWeapon>(DefaultInventoryClass[i], SpawnInfo);
			AddWeapon(NewWeapon);
			EquipWeapon(NewWeapon);
			//UE_LOG(LogTemp, Warning, TEXT("FirstWeapon on!"));

			if (!FirstSlot)
			{
				FirstWeapon = NewWeapon;
				FirstWeaponSubclass = DefaultInventoryClass[i];
			}
			else if (!SecondSlot)
			{
				SecondWeapon = NewWeapon;
				SecondWeaponSubclass = DefaultInventoryClass[i];
			}
		}		
		//UE_LOG(LogTemp, Warning, TEXT("DefaultInventoryClass not set!"));		
	}	
	if (Inventory.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory.Num() number: %d !!"), Inventory.Num());
		//UE_LOG(LogTemp, Warning, TEXT("InventoryNum set!"));
		//EquipWeapon(Inventory[Weaponnum]);
		//EquipWeapon(WeaponItem);
	}
	if (!FirstSlot)
	{
		FirstOnWeaponChanged.Broadcast();	
	}
	else if (!SecondSlot)
	{
		SecondOnWeaponChanged.Broadcast();
	}
}

FName APlayerCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

FName APlayerCharacter::GetWeaponSecondAttachPoint() const
{
	return WeaponSecondAttachPoint;
}



//이부분에서 무기정보를 줘야 함
void APlayerCharacter::EquipWeapon(AMyTestWeapon* Weapon)
{
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipWeapon set!"));
		SetCurrentWeapon(Weapon, CurrentWeapon);

	}
	/*
	if (Weapon)
	{
		Weapon->SetOwningPawnCharacter(this);
		Weapon->OnEquip(Weapon);
	}
	*/
}



//최종적으로 붙히는F
void APlayerCharacter::SetCurrentWeapon(AMyTestWeapon* NewWeapon, AMyTestWeapon* LastWeapon)
{
	AMyTestWeapon* LocalLastWeapon = NULL;

	//최근무기가 있었다면
	if (LastWeapon)
	{
		LocalLastWeapon = LastWeapon;
		NewWeapon->SetOwningPawnCharacter(this);
		NewWeapon->OnEquip(LastWeapon);

	}
	//마지막 무기가없다면
	else
	{
		if (NewWeapon)
		{
			NewWeapon->SetOwningPawnCharacter(this);
			NewWeapon->OnEquip(LastWeapon);
			CurrentWeapon = NewWeapon;
		}
	}

	
}


void APlayerCharacter::AddWeapon(AMyTestWeapon* Weapon)
{
	if (Weapon)
	{
		Inventory.AddUnique(Weapon);
	}
}

USkeletalMeshComponent* APlayerCharacter::GetSpecificPawnMesh() const
{
	return GetMesh();
}



void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };


	float LookDirection = FVector::DotProduct(GetActorForwardVector(),GetViewRotation().Vector());

	if (LookDirection > 0) {

		//시야각(빨간줄)
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);


		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable )
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
		NoInteractableFound();
	
}

void APlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void APlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		//Hide Interaction Widget on the HUD
		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void APlayerCharacter::BeginInteract()
{
	//verify nothing has chaanget with the interacttable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			UE_LOG(LogTemp, Warning, TEXT("BeginInteract: Valid Interactable Found"));
			TargetInteractable->BeginInteract();
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &APlayerCharacter::Interact, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void APlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void APlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

/*
UInventoryComponent* APlayerCharacter::GetInventory()
{


	return PlayerInventory;
	
}
*/

void APlayerCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}

}


void APlayerCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}



void APlayerCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };

		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null"));
	}
}

void APlayerCharacter::IncreaseHealth(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->MyHealth += 30;
	if (PlayerCharacter->MyHealth > PlayerCharacter->MyMaxHealth)
	{
		PlayerCharacter->MyHealth = PlayerCharacter->MyMaxHealth;
	}
	UE_LOG(LogTemp, Warning, TEXT("IncreaseHealth!"));
}

void APlayerCharacter::SetWeaponReference(UItemBase* Item)
{
	WeaponItem = Item;
	IsChange = true;
	
	

	//FirstWeapon
}

UItemBase* APlayerCharacter::GetWeaponReference()
{
	return WeaponItem;
}



void APlayerCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && IsDuringAttack == false)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && IsDuringAttack == false)
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRot(0, Rot.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::StartJump()
{
	bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
	bPressedJump = false;
}

void APlayerCharacter::OnRightMouseClick()
{
	bRightMouseClicked = true;
}

void APlayerCharacter::OnRightMouseRelease()
{
	bRightMouseClicked = false;
}

void APlayerCharacter::Turn(float Value)
{
	// 마우스 X 이동에 대한 회전 속도를 조절
	AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUp(float Value)
{
	// 마우스 Y 이동에 대한 회전 속도를 조절
	AddControllerPitchInput(Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Slash()
{
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraActor, GetActorLocation());



	/*

	// 애니메이션 몽타주가 정상적으로 종료된 경우에만 실행
	if (MySlash)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(90.0f, 0.0f, 90.0f));
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			World->SpawnActor<AMySlashAura>(MySlash, SpawnLocation, SpawnRotation);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No Slash Actor");
	}
	*/
	
	
	


	// 애니메이션 몽타주를 재생하고 애니메이션이 끝나면 OnSlashMontageEnded 함수를 호출하도록 설정
	if (FirstWeapon)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && SlashAnim)
		{
			bIsSlashing = true;
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &APlayerCharacter::SlashEnd);
			AnimInstance->Montage_Play(SlashAnim);
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Slash1");
			AnimInstance->Montage_SetEndDelegate(EndDelegate, SlashAnim);

			if (SlashSound)
			{
				// Set a timer to play the sound after 0.5 seconds
				GetWorld()->GetTimerManager().SetTimer(SlashSoundTimerHandle, this, &APlayerCharacter::PlaySlashSound, 0.5f, false);
			}

			
		}


	}
	//PlayAnimMontage(SlashAnim);
}

void APlayerCharacter::SlashEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsSlashing = false;

	
	if (!bInterrupted)
	{
		// 애니메이션 몽타주가 정상적으로 종료된 경우에만 실행
		if (MySlash)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Slash2");
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(FVector(90.0f, 0.0f, 90.0f));
			//UWorld* const World = GetWorld();
			MySlashInstance = GetWorld()->SpawnActor<AMySlashAura>(MySlash, GetActorLocation(), GetActorRotation());
			if (MySlashInstance != nullptr)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Slash3");
				//World->SpawnActor<AMySlashAura>(MySlash, SpawnLocation, SpawnRotation);
				MySlashInstance->SetOwningSlash(this);
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No Slash Actor");
		}
	}
}

void APlayerCharacter::Roll()
{
	if (IsRolling) return; // 이미 구르고 있는 중이라면 중복 실행 방지

	IsRolling = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "roll!");
	PlayAnimMontage(RollAnim, 1.0f);

	// 1초 후에 ResetIsRolling 함수를 호출하여 IsRolling을 false로 설정
	GetWorld()->GetTimerManager().SetTimer(RollTimerHandle, this, &APlayerCharacter::ResetIsRolling, 1.0f, false);

}

void APlayerCharacter::ResetIsRolling()
{
	IsRolling = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Roll ended!");
}

void APlayerCharacter::AOW()
{
	if (NowStamina > 50) // 스테미나가 10 이상일 때만 실행
	{
		AOWNUM = true;
		PlayAnimMontage(AOWAnim);
		NowStamina -= 50;

		FTimerHandle CameraShakeTimerHandle;
		GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &APlayerCharacter::TriggerCameraShake, 0.8f, false);
	}
}

void APlayerCharacter::TriggerCameraShake()
{
	if (SwordCamerashake) // SwordCamerashake가 유효한지 확인
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(SwordCamerashake);
	}
}

void APlayerCharacter::FocusOnTarget(AActor* TargetActor)
{
	if (!TargetActor || !TargetActor->IsValidLowLevel()) return;  // TargetActor 유효성 확인

	// 적의 위치를 가져옴
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector PlayerLocation = GetActorLocation();

	// 적을 바라보는 회전값을 계산
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);

	// 플레이어 캐릭터의 Yaw 값만 적을 향하도록 변경
	FRotator NewRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
	SetActorRotation(NewRotation);
}

void APlayerCharacter::LockOn()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Lockon!");
	/*
	if (LockedOnTarget) {
		if (LockedOnTarget->IsA(AMyBotCharacter_3::StaticClass())) {


			AMyBotCharacter_3* TargetBot = Cast<AMyBotCharacter_3>(LockedOnTarget);
			TargetBot->SetLockOnWidgetVisibility(false);
		}
	}
	*/
	if (bIsLockedOn)
	{
		// 락온 해제: CollisionSphere와 락온 표시 제거
		if (CollisionSphere)
		{
			CollisionSphere->DestroyComponent();
			CollisionSphere = nullptr;

			if (LockedOnTarget) {
				AMyBotCharacter_3* TargetBot = Cast<AMyBotCharacter_3>(LockedOnTarget);
				TargetBot->SetLockOnWidgetVisibility(false);
				TargetBoss = TargetBot;
			}
		}

		

		GetWorldTimerManager().ClearTimer(LockOnFocusTimer);

		bIsLockedOn = false;
		LockedOnTarget = nullptr;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Lockon Released!");
		return;
	}

	// 락온 활성화
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Lockon Activated!");

	// SphereComponent를 동적으로 생성하여 특정 범위 내 캐릭터를 감지
	CollisionSphere = NewObject<USphereComponent>(this);
	if (CollisionSphere)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "CollisionSphere Activated!");
		CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		CollisionSphere->InitSphereRadius(1000.0f);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		CollisionSphere->RegisterComponent();

		

		// 오버랩 이벤트를 바인딩
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	}

	bIsLockedOn = true;


	
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && (!OtherActor || !OtherActor->IsA(ACharacter::StaticClass()))) return;

	if (OtherActor->IsA(APlayerCharacter::StaticClass())) {
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Locked On Target: %s"), *OtherActor->GetName()));

	// 락온 대상 설정
	LockedOnTarget = Cast<ACharacter>(OtherActor);

	
	// 락온 활성화
	if (LockedOnTarget)
	{
		AMyBotCharacter_3* TargetBot = Cast<AMyBotCharacter_3>(LockedOnTarget);
		if (TargetBot)
		{
			TargetBoss = TargetBot;
			TargetBot->SetLockOnWidgetVisibility(true);
		}

		bIsLockedOn = true;
	}

	
	
}

void APlayerCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "OnMontageNotifyBegin on");
	if (NotifyName == "DamageStart")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "NotifyName = DamageStart");
		DamageApply = true;
	}
}

void APlayerCharacter::OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "DamageEnd")
	{

		DamageApply = false;
		AOWNUM = false;
	}
}






void APlayerCharacter::Load()
{
	UGameInstance_Player* GameInstance = Cast<UGameInstance_Player>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetPlayerReference(this);
	}

	GameInstance->LoadPlayerData();
}

void APlayerCharacter::Save()
{
	UGameInstance_Player* GameInstance = Cast<UGameInstance_Player>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SetPlayerReference(this);
	}
	//GameInstance->MyFirstWeapon = FirstWeaponSubclass;
	//GameInstance->MySecondWeapon = SecondWeaponSubclass;
	GameInstance->SavePlayerData();
}

void APlayerCharacter::OnLevelLoaded()
{
	Load();
	UE_LOG(LogTemp, Warning, TEXT("Level has been loaded!"));
}

void APlayerCharacter::AttackMelee()
{
	//그로기 상태 강공 구현
	if (GroggyState && GroggyAttackAttempt) {
		if (NowStamina > 20) // 상대 위치 조건과 스태미나 확인
		{
			PlayAnimMontage(GroggyAttackAnim, 1.0f);
			NowStamina -= 20;
			GroggyState = false;
			GroggyAttackAttempt = false;



			// 보스의 남은 그로기 시간에 4초 추가
			if (AMyBotCharacter_3* Boss = Cast<AMyBotCharacter_3>(TargetBoss)) // TargetBoss가 보스 캐릭터임을 가정
			{
				float RemainingTime = Boss->GetWorldTimerManager().GetTimerRemaining(Boss->GroggyTimerHandle);
				if (RemainingTime > 0.0f)
				{
					// 남은 시간에 4초를 더하여 타이머를 재설정
					Boss->GetWorldTimerManager().SetTimer(Boss->GroggyTimerHandle, Boss, &AMyBotCharacter_3::GroggyEndState, RemainingTime + 4.0f, false);

					// 1.5초 뒤에 데미지를 적용하는 타이머 설정
					FTimerHandle DamageTimerHandle;
					GetWorldTimerManager().SetTimer(DamageTimerHandle, [Boss, this]()
						{
							UGameplayStatics::ApplyDamage(Boss, 50, this->GetController(), nullptr, UDamageType::StaticClass());
						}, 1.5f, false);
				}
			}

			// 카메라 흔들림 효과
			FTimerHandle CameraShakeTimerHandle;
			GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &APlayerCharacter::TriggerCameraShake, 1.5f, false);

			return; // 특수공격 후에는 일반 공격으로 넘어가지 않도록 함
		}
	}

	switch (ComboAttack_Num)
	{
	case 0:
		if (NowStamina > 10) // 스테미나가 10 이상일 때만 실행
		{
			//UGameplayStatics::PlaySoundAtLocation(this, SlashSound, GetActorLocation());
			PlayAnimMontage(AttackMeleeAnim1, 1.0f);
			IsDuringAttack = true;
			ComboAttack_Num++;
			NowStamina -= 10;

			
		}
		break;
	case 1:
		if (NowStamina > 20) {
			//UGameplayStatics::PlaySoundAtLocation(this, SlashSound, GetActorLocation());
			PlayAnimMontage(AttackMeleeAnim2, 1.0f);
			IsDuringAttack = true;
			ComboAttack_Num++;
			NowStamina -= 20;
			
		}
		break;
	case 2:
		if (NowStamina > 30) {
			//UGameplayStatics::PlaySoundAtLocation(this, SlashSound, GetActorLocation());
			PlayAnimMontage(AttackMeleeAnim3, 1.0f);
			IsDuringAttack = true;
			ComboAttack_Num++;
			NowStamina -= 30;
			
			FTimerHandle CameraShakeTimerHandle;
			GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &APlayerCharacter::TriggerCameraShake, 0.5f, false);
		}
		break;
	default:
		ComboAttack_Num = 0;
	}

	FTimerHandle TH_Attack_End;
	GetWorldTimerManager().SetTimer(TH_Attack_End, this, &APlayerCharacter::AttackMeleeEnd, 3.0f, false);
}

void APlayerCharacter::AttackMeleeEnd()
{
	
	IsDuringAttack = false;
	ComboAttack_Num = 0;
}

void APlayerCharacter::ComboMelee1()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Combo1!");
	PlayAnimMontage(ComboMelee1Anim, 1.0f);
	IsDuringAttack = true;

	FTimerHandle TH_Combo_End;
	GetWorldTimerManager().SetTimer(TH_Combo_End, this, &APlayerCharacter::ComboMelee1End, 1.7f, false);
}

void APlayerCharacter::ComboMelee2()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Combo2!");
	PlayAnimMontage(ComboMelee2Anim, 1.0f);
	IsDuringAttack = true;

	FTimerHandle TH_Combo_End;
	GetWorldTimerManager().SetTimer(TH_Combo_End, this, &APlayerCharacter::ComboMelee2End, 1.7f, false);

}

void APlayerCharacter::ComboMelee1End()
{
	IsDuringAttack = false;
}

void APlayerCharacter::ComboMelee2End()
{
	IsDuringAttack = false;
}

void APlayerCharacter::Block()
{
	if (NowStamina > 10) // 스테미나가 10 이상일 때만 실행
	{
		IsBlock = true;
		PlayAnimMontage(BlockAnim, 1.0f);

		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &APlayerCharacter::BlockEnd);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, BlockAnim);

		NowStamina -= 10;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Block!");
	}
}

void APlayerCharacter::BlockEnd(UAnimMontage* Montage, bool bInterrupted)
{
	IsBlock = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Block Ended!");
}

void APlayerCharacter::BackEvade()
{
	// 이미 회피 중이라면 함수를 종료하여 중복 실행을 방지
	if (bIsEvading) return;

	// BackEvadeAnim 애니메이션이 유효한지 확인
	if (BackEvadeAnim)
	{
		// 애니메이션 재생
		PlayAnimMontage(BackEvadeAnim);

		// 회피 중 상태로 전환
		bIsEvading = true;

		// 애니메이션이 끝났을 때 호출되는 델리게이트 설정
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &APlayerCharacter::OnBackEvadeAnimEnded);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, BackEvadeAnim);
	}
}

// 애니메이션 재생 완료 시 호출되는 함수
void APlayerCharacter::OnBackEvadeAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 회피 상태 해제
	bIsEvading = false;
}

void APlayerCharacter::ShowDamageText(float DamageAmount)
{
	if (DamageTextWidgetClass)
	{
		UPlayerHitDamage* HitDamageWidget = CreateWidget<UPlayerHitDamage>(GetWorld(), DamageTextWidgetClass);

		if (HitDamageWidget)
		{
			FVector SocketLocation = GetMesh()->GetSocketLocation(FName("DamageWidget"));

			UWidgetComponent* DamageWidgetComponent = NewObject<UWidgetComponent>(this);
			DamageWidgetComponent->SetWidget(HitDamageWidget);
			DamageWidgetComponent->SetWorldLocation(SocketLocation);
			DamageWidgetComponent->SetDrawSize(FVector2D(20, 20));
			DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			DamageWidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			DamageWidgetComponent->RegisterComponent();

			HitDamageWidget->SetHitDamage(DamageAmount);

			// 애니메이션 재생
			HitDamageWidget->PlayDamageTextAnimation();

			// 위젯 제거와 이동 처리
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DamageWidgetComponent]()
				{
					if (DamageWidgetComponent)
					{
						DamageWidgetComponent->DestroyComponent();
					}
				}, 1.0f, false);

			FVector StartLocation = SocketLocation;
			FVector EndLocation = StartLocation + FVector(0, 0, 50);

			FTimerHandle AnimationTimerHandle;
			float Duration = 1.0f;

			// DeltaSeconds를 미리 계산
			float DeltaTime = GetWorld()->GetDeltaSeconds();

			// 타이머 설정
			GetWorld()->GetTimerManager().SetTimer(AnimationTimerHandle, [DamageWidgetComponent, StartLocation, EndLocation, DeltaTime]()
				{
					if (DamageWidgetComponent)
					{
						FVector CurrentLocation = DamageWidgetComponent->GetComponentLocation();
						FVector NewLocation = FMath::VInterpTo(CurrentLocation, EndLocation, DeltaTime, 2.0f);

						DamageWidgetComponent->SetWorldLocation(NewLocation);
					}
				}, DeltaTime, false);
		}
	}
}

void APlayerCharacter::AddEXP(float EXP)
{
	MyEXP += EXP;
}

AMyTestWeapon* APlayerCharacter::GetFirstWeapon()
{
	return FirstWeapon;
}



void APlayerCharacter::PlaySlashSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, SlashSound, GetActorLocation());
}




float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (this->GetCannotDamaged()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Rolling not Damaged");
	}
	if (IsBlock)
	{
		//사운드 재생
		UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation());

		if (BlockEffect)
		{
			FName SocketName = TEXT("BlockNia");  // 파티클을 재생할 소켓 이름

			UNiagaraFunctionLibrary::SpawnSystemAttached(
				BlockEffect,                // 재생할 파티클 시스템
				GetMesh(),                  // 파티클을 붙일 메쉬 (캐릭터의 스켈레탈 메쉬)
				SocketName,                 // 파티클을 생성할 소켓 이름
				FVector::ZeroVector,        // 위치 오프셋 (기본값 사용)
				FRotator::ZeroRotator,      // 회전 오프셋 (기본값 사용)
				EAttachLocation::SnapToTargetIncludingScale, // 소켓에 위치와 스케일을 맞춤
				true                        // 파티클이 끝날 때 자동으로 삭제
			);
		}
		this->ShowDamageText(0);
		TargetBoss->Groggy += 1;
		//그로기 스텟증가
		
	}
	else
	{
		if (DamageCauser->IsA(ABossSill2_Actor::StaticClass()))
		{
			if (myGetDamage > 0.f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Get Damage");
				this->ShowDamageText(10);
				MyHealth -= myGetDamage;
				PlayAnimMontage(BeHitAnimMontage);
			}
			if (MyHealth <= 0.f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 1!");
				//Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
			}
			return myGetDamage;
		}
		else if (EventInstigator->GetPawn()->IsA(AMyBotCharacter_3::StaticClass()))
		{
			AMyBotCharacter_3* BotCharacter = Cast<AMyBotCharacter_3>(EventInstigator->GetPawn());

			if (myGetDamage > 0.f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Get Damage");
				this->ShowDamageText(Damage);
				MyHealth -= myGetDamage;
				PlayAnimMontage(BeHitAnimMontage);
			}
			if (MyHealth <= 0.f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 1!");
				//Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
			}
			return myGetDamage;
		}
		else if (EventInstigator->GetPawn()->IsA(AMyBotCharacter_2::StaticClass()))
		{
			AMyBotCharacter_2* BotCharacter = Cast<AMyBotCharacter_2>(EventInstigator->GetPawn());

			if (myGetDamage > 0.f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Get Damage");
				this->ShowDamageText(Damage);
				MyHealth -= myGetDamage;
				PlayAnimMontage(BeHitAnimMontage);
			}
			if (MyHealth <= 0.f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Die Step 1!");
				//Die(myGetDamage, DamageEvent, EventInstigator, DamageCauser);
			}
			return myGetDamage;
		}
	}
	
	
	
	
	return myGetDamage;
}


