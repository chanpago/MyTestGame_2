// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractionInterface.h"
#include "PlayerCharacter.generated.h"

class AMyTestWeapon;
class AMainHUD;
class UInventoryComponent;
class UFirstWeaponSlot;
class UItemBase;
class UHpExpBar;
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class AMySlashAura;
class AMyBotCharacter_3;

DECLARE_MULTICAST_DELEGATE(FOnWeaponChanged);

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};


UCLASS()
class MYTESTGAME_2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float MyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float MyMaxHealth = 100.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyLevel = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyMaxEXP = 15.f * MyLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyEXP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MyHPnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MyHPbarText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MyEXPnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NowStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Staminanum;

	class UHpExpBar* HPEXPWidgetComponent;

	class UWidgetComponent* WidgetComponent;

	class UItemBase* WeaponItem;

	class UFirstWeaponSlot* FirstWeaponInventory;

	bool IsChange;

	bool FirstSlot;

	bool SecondSlot;

	AMyTestWeapon* FirstWeapon;

	AMyTestWeapon* SecondWeapon;

	AMyTestWeapon* FirstWeaponInstance;

	AMyTestWeapon* SecondWeaponInstance;

	class AMyTestWeapon* CurrentWeapon;
	//AMyTestWeapon* Weapon;

	FOnWeaponChanged FirstOnWeaponChanged;

	FOnWeaponChanged SecondOnWeaponChanged;



	//슬래쉬 이팩트 및 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	TSubclassOf<class AMySlashAura> MySlash;

	// Pointer to the instance of AMySlashAura

	AMySlashAura* MySlashInstance;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* SlashAnim;


	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMeleeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMeleeAnim1;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMeleeAnim2;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMeleeAnim3;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* RollAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* GroggyAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AOWAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BlockAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* ComboMelee1Anim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* ComboMelee2Anim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BackEvadeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* SideEvadeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BeHitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ForwardRollAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* BackwardRollAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* RightRollAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* LeftRollAnim;

	//방어시 사운드, 이팩트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* BlockEffect;  // 막았을 때 나오는 이펙트
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDuringAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DamageApply = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBlock = false;

	int ComboAttack_Num;

	bool IsRolling = false;

	FTimerHandle RollTimerHandle;

	bool AOWNUM;

	bool CannotDamaged;

	bool GroggyState = false;

	bool GroggyAttackAttempt = true;

	bool GroggyAttack = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> SwordCamerashake;

	AMyBotCharacter_3* TargetBoss;

	

	//=========================================================================
	// FUCTIONS
	//=========================================================================

	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void BeginPlay();

	//UPROPERTY(EditAnywhere, Category = Input)
	//UInputMappingContext* PlayerContext;

	//UPROPERTY(EditAnywhere, Category = Input)
	//UInputMappingContext* PlayerContext;

	void SetCannotDamaged(bool value);

	bool GetCannotDamaged();

	int GetComboAttackNum();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FName GetWeaponAttachPoint() const;

	FName GetWeaponSecondAttachPoint() const;

	//void RemoveWeapon() const;

	void EquipWeapon(class AMyTestWeapon* Weapon);

	void FocusOnTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void LockOn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* CollisionSphere;

	//TArray<AActor*>APlayerCharacter::GetCharactersInRange();

	USkeletalMeshComponent* GetSpecificPawnMesh() const;


	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	FORCEINLINE UInventoryComponent* GetInventory()const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	void IncreaseHealth(APlayerCharacter* PlayerCharacter);

	//FORCEINLINE void SetWeaponInventory(UFirstWeaponSlot* WeaponInventory){ FirstWeaponInventory = WeaponInventory;  };
	FORCEINLINE UFirstWeaponSlot* GetWeaponInventory() const { return FirstWeaponInventory; };

	void SetWeaponReference(UItemBase* Item);

	UItemBase* GetWeaponReference();



	FORCEINLINE UItemBase* GetWeaponReference() const { return WeaponItem; };

	
	void SetCurrentWeapon(class AMyTestWeapon* NewWeapon, class AMyTestWeapon* LastWeapon);

	void SpawnDefaultInventory();

	void AddWeapon(class AMyTestWeapon* Weapon);


	//슬래쉬 이팩트
	UFUNCTION(BlueprintCallable)
	void Slash();

	UFUNCTION(BlueprintCallable)
	void SlashEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void Roll();

	UFUNCTION()
	void ResetIsRolling();

	UFUNCTION(BlueprintCallable)
	void AOW();

	void TriggerCameraShake();

	//void UpdateLockOnFocus();

	//로드 세이브

	void Load();
	void Save();

	void OnLevelLoaded();


	//공격
	UFUNCTION(BlueprintCallable)
	void AttackMelee();
	
	UFUNCTION(BlueprintCallable)
	void AttackMeleeEnd();

	//combo
	UFUNCTION(BlueprintCallable)
	void ComboMelee1();

	UFUNCTION(BlueprintCallable)
	void ComboMelee2();

	UFUNCTION(BlueprintCallable)
	void ComboMelee1End();

	UFUNCTION(BlueprintCallable)
	void ComboMelee2End();

	//방어
	UFUNCTION(BlueprintCallable)
	void Block();

	UFUNCTION(BlueprintCallable)
	void BlockEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void BackEvade();

	UFUNCTION(BlueprintCallable)
	void OnBackEvadeAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	//화면데미지
	void ShowDamageText(float DamageAmount);

	//경험치증가
	void AddEXP(float EXP);

	//첫번째 무기 반환ㅇ
	AMyTestWeapon* GetFirstWeapon();

	//데미지받음
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY()
	AMainHUD* HUD;


	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float TurnRate = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float LookUpRate = 45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;


	//UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	//UFirstWeaponSlot* FirstWeaponInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponSecondAttachPoint;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TArray<TSubclassOf<class AMyTestWeapon>>DefaultInventoryClass;

	TArray<class AMyTestWeapon*>Inventory;

	bool  bIsSlashing;

	TSubclassOf<class AMyTestWeapon> FirstWeaponSubclass;

	TSubclassOf<class AMyTestWeapon> SecondWeaponSubclass;

	// 나이아가라 이펙트 시스템
	UPROPERTY(EditDefaultsOnly, Category = "MyFX")
	UNiagaraSystem* NiagaraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SlashSound;

	FTimerHandle SlashSoundTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHitDamage> DamageTextWidgetClass;

	// 애니메이션이 재생되는 동안 입력을 차단하기 위한 변수
	bool bIsEvading = false;

	//=========================================================================
	// FUCTIONS
	//=========================================================================


	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();

	void BeginInteract();
	void EndInteract();
	void Interact();



	void MoveForward(float value);
	void MoveRight(float value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	bool bRightMouseClicked;
	void OnRightMouseClick();
	void OnRightMouseRelease();
	void Turn(float Value);
	void LookUp(float Value);

	void ToggleMenu();

	void PlaySlashSound();

private:
	FTimerHandle NiagaraTimerHandle;

	FName SlashSocketName = "hand_r_socket"; // 소켓 이름 설정
};
