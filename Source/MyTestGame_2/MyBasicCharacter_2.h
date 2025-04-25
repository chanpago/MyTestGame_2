 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Controller.h"
#include "MyBasicCharacter_2.generated.h"

UCLASS()
class MYTESTGAME_2_API AMyBasicCharacter_2 : public ACharacter
{
	GENERATED_BODY()

public:

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	// Sets default values for this character's properties

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float MyHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float MyMaxHealth=100.f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyLevel = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyMaxEXP = 15.f * MyLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyEXP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float MyDamage = 10.f;

	bool IsBotAttack = false;


	UPROPERTY(EditDefaultsOnly, Category = "MyFX")
	UParticleSystem* ChargeFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	TSubclassOf<class AMySlashAura> MySlash;

	// Pointer to the instance of AMySlashAura
	
	AMySlashAura* MySlashInstance;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* AttackMeleeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDuringAttack = false;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* ComboMelee1Anim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* ComboMelee2Anim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* EvadeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* ChargeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* SlashAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* SlashAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MyHPnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString MyHPbarText;

	//=========================================================================
	// FUCTIONS
	//=========================================================================

	AMyBasicCharacter_2();

	USkeletalMeshComponent* GetSpecificPawnMesh() const;

	FName GetWeaponAttachPoint() const;

	void EquipWeapon(class AMyTestWeapon* Weapon);

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	UFUNCTION(BlueprintCallable)
	void Slash();

	UFUNCTION(BlueprintCallable)
	void SlashEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void Evade();

	void DeathAnimationEnd();

	float GetHealth() const;

	float GetMaxHealth() const;

	void SetHealth(float const NewHealth);

	void SetLevel(float const Level);

	//화면데미지
	void ShowDamageText(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	float GetHealthValue();


protected:

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPoint;

	TArray<class AMyTestWeapon*>Inventory;

	class AMyTestWeapon* CurrentWeapon;

	class UWidgetComponent* HPEXPWidgetComponent;

	class UWidgetComponent* HpWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHitDamage> DamageTextWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<class AMyTestWeapon>>DefaultInventoryClass;

	class UWidgetComponent* WidgetComponent;



	//=========================================================================
	// FUCTIONS
	//=========================================================================


	void AddWeapon(class AMyTestWeapon* Weapon);

	void SetCurrentWeapon(class AMyTestWeapon* NewWeapon, class AMyTestWeapon* LastWeapon);

	void SpawnDefaultInventory();



private:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	class UAIUAIPerceptionStimuliSourceComponent* stimulus;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void SetupStimulus();
};
