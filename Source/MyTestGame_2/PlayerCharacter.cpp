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

// ���µ� ĳ���Ϳ� ��� ���� ǥ���ϱ� ���� ���� ������Ʈ ������ �߰�
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

	// ī�޶� �浹 ���� ��Ȱ��ȭ
	CameraBoom->bDoCollisionTest = false; // ī�޶� �浹 �׽�Ʈ ��Ȱ��ȭ

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

	
	// ĸ�� ������Ʈ�� �浹 ����
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	// ��Ʈ ������Ʈ�� �����Ǿ����� Ȯ��
	RootComponent = GetCapsuleComponent();

	// �޽��� ���� �ùķ��̼� ��Ȱ��ȭ
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


	// ���� �� �̸� ��������
	FString CurrentMapName = GetWorld()->GetMapName();
	CurrentMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UE_LOG(LogTemp, Warning, TEXT("Current Map Name: %s"), *CurrentMapName);

	// �� �̸��� "Demonstration"�� �ƴ� ��쿡�� Load() ȣ��
	if (CurrentMapName == "Demonstration")
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentMapName == Demonstration "));
	}
	else
	{
		// �÷��̾� ���� �ε�
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


	// ���� ���¿��� ����� ���Ǿ� �׸���
	if (bIsLockedOn && CollisionSphere)
	{
		//DrawDebugSphere(GetWorld(), GetActorLocation(), CollisionSphere->GetScaledSphereRadius(), 32, FColor::Green, false, -1.0f, 0, 2.0f);
	}

	// ���� ���¿��� ���� ����
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



//�̺κп��� ���������� ��� ��
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



//���������� ������F
void APlayerCharacter::SetCurrentWeapon(AMyTestWeapon* NewWeapon, AMyTestWeapon* LastWeapon)
{
	AMyTestWeapon* LocalLastWeapon = NULL;

	//�ֱٹ��Ⱑ �־��ٸ�
	if (LastWeapon)
	{
		LocalLastWeapon = LastWeapon;
		NewWeapon->SetOwningPawnCharacter(this);
		NewWeapon->OnEquip(LastWeapon);

	}
	//������ ���Ⱑ���ٸ�
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

		//�þ߰�(������)
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
	// ���콺 X �̵��� ���� ȸ�� �ӵ��� ����
	AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUp(float Value)
{
	// ���콺 Y �̵��� ���� ȸ�� �ӵ��� ����
	AddControllerPitchInput(Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Slash()
{
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraActor, GetActorLocation());



	/*

	// �ִϸ��̼� ��Ÿ�ְ� ���������� ����� ��쿡�� ����
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
	
	
	


	// �ִϸ��̼� ��Ÿ�ָ� ����ϰ� �ִϸ��̼��� ������ OnSlashMontageEnded �Լ��� ȣ���ϵ��� ����
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
		// �ִϸ��̼� ��Ÿ�ְ� ���������� ����� ��쿡�� ����
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
	if (IsRolling) return; // �̹� ������ �ִ� ���̶�� �ߺ� ���� ����

	IsRolling = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "roll!");
	PlayAnimMontage(RollAnim, 1.0f);

	// 1�� �Ŀ� ResetIsRolling �Լ��� ȣ���Ͽ� IsRolling�� false�� ����
	GetWorld()->GetTimerManager().SetTimer(RollTimerHandle, this, &APlayerCharacter::ResetIsRolling, 1.0f, false);

}

void APlayerCharacter::ResetIsRolling()
{
	IsRolling = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Roll ended!");
}

void APlayerCharacter::AOW()
{
	if (NowStamina > 50) // ���׹̳��� 10 �̻��� ���� ����
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
	if (SwordCamerashake) // SwordCamerashake�� ��ȿ���� Ȯ��
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(SwordCamerashake);
	}
}

void APlayerCharacter::FocusOnTarget(AActor* TargetActor)
{
	if (!TargetActor || !TargetActor->IsValidLowLevel()) return;  // TargetActor ��ȿ�� Ȯ��

	// ���� ��ġ�� ������
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector PlayerLocation = GetActorLocation();

	// ���� �ٶ󺸴� ȸ������ ���
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);

	// �÷��̾� ĳ������ Yaw ���� ���� ���ϵ��� ����
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
		// ���� ����: CollisionSphere�� ���� ǥ�� ����
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

	// ���� Ȱ��ȭ
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Lockon Activated!");

	// SphereComponent�� �������� �����Ͽ� Ư�� ���� �� ĳ���͸� ����
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

		

		// ������ �̺�Ʈ�� ���ε�
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

	// ���� ��� ����
	LockedOnTarget = Cast<ACharacter>(OtherActor);

	
	// ���� Ȱ��ȭ
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
	//�׷α� ���� ���� ����
	if (GroggyState && GroggyAttackAttempt) {
		if (NowStamina > 20) // ��� ��ġ ���ǰ� ���¹̳� Ȯ��
		{
			PlayAnimMontage(GroggyAttackAnim, 1.0f);
			NowStamina -= 20;
			GroggyState = false;
			GroggyAttackAttempt = false;



			// ������ ���� �׷α� �ð��� 4�� �߰�
			if (AMyBotCharacter_3* Boss = Cast<AMyBotCharacter_3>(TargetBoss)) // TargetBoss�� ���� ĳ�������� ����
			{
				float RemainingTime = Boss->GetWorldTimerManager().GetTimerRemaining(Boss->GroggyTimerHandle);
				if (RemainingTime > 0.0f)
				{
					// ���� �ð��� 4�ʸ� ���Ͽ� Ÿ�̸Ӹ� �缳��
					Boss->GetWorldTimerManager().SetTimer(Boss->GroggyTimerHandle, Boss, &AMyBotCharacter_3::GroggyEndState, RemainingTime + 4.0f, false);

					// 1.5�� �ڿ� �������� �����ϴ� Ÿ�̸� ����
					FTimerHandle DamageTimerHandle;
					GetWorldTimerManager().SetTimer(DamageTimerHandle, [Boss, this]()
						{
							UGameplayStatics::ApplyDamage(Boss, 50, this->GetController(), nullptr, UDamageType::StaticClass());
						}, 1.5f, false);
				}
			}

			// ī�޶� ��鸲 ȿ��
			FTimerHandle CameraShakeTimerHandle;
			GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &APlayerCharacter::TriggerCameraShake, 1.5f, false);

			return; // Ư������ �Ŀ��� �Ϲ� �������� �Ѿ�� �ʵ��� ��
		}
	}

	switch (ComboAttack_Num)
	{
	case 0:
		if (NowStamina > 10) // ���׹̳��� 10 �̻��� ���� ����
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
	if (NowStamina > 10) // ���׹̳��� 10 �̻��� ���� ����
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
	// �̹� ȸ�� ���̶�� �Լ��� �����Ͽ� �ߺ� ������ ����
	if (bIsEvading) return;

	// BackEvadeAnim �ִϸ��̼��� ��ȿ���� Ȯ��
	if (BackEvadeAnim)
	{
		// �ִϸ��̼� ���
		PlayAnimMontage(BackEvadeAnim);

		// ȸ�� �� ���·� ��ȯ
		bIsEvading = true;

		// �ִϸ��̼��� ������ �� ȣ��Ǵ� ��������Ʈ ����
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &APlayerCharacter::OnBackEvadeAnimEnded);
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, BackEvadeAnim);
	}
}

// �ִϸ��̼� ��� �Ϸ� �� ȣ��Ǵ� �Լ�
void APlayerCharacter::OnBackEvadeAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// ȸ�� ���� ����
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

			// �ִϸ��̼� ���
			HitDamageWidget->PlayDamageTextAnimation();

			// ���� ���ſ� �̵� ó��
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

			// DeltaSeconds�� �̸� ���
			float DeltaTime = GetWorld()->GetDeltaSeconds();

			// Ÿ�̸� ����
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
		//���� ���
		UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation());

		if (BlockEffect)
		{
			FName SocketName = TEXT("BlockNia");  // ��ƼŬ�� ����� ���� �̸�

			UNiagaraFunctionLibrary::SpawnSystemAttached(
				BlockEffect,                // ����� ��ƼŬ �ý���
				GetMesh(),                  // ��ƼŬ�� ���� �޽� (ĳ������ ���̷�Ż �޽�)
				SocketName,                 // ��ƼŬ�� ������ ���� �̸�
				FVector::ZeroVector,        // ��ġ ������ (�⺻�� ���)
				FRotator::ZeroRotator,      // ȸ�� ������ (�⺻�� ���)
				EAttachLocation::SnapToTargetIncludingScale, // ���Ͽ� ��ġ�� �������� ����
				true                        // ��ƼŬ�� ���� �� �ڵ����� ����
			);
		}
		this->ShowDamageText(0);
		TargetBoss->Groggy += 1;
		//�׷α� ��������
		
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


