// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBasicCharacter_2.h"
#include "Animation/AnimMontage.h"
#include "CombatInterface.h"
#include "MyBotCharacter_2.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API AMyBotCharacter_2 : public AMyBasicCharacter_2, public ICombatInterface
{
	GENERATED_BODY()

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	AMyBotCharacter_2();

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior;

	int MeleeAttack_Implementation()override;

	UFUNCTION(BlueprintCallable)
	void MeleeAttackEnd();

	UAnimMontage* GetMontage() const;

	virtual void PostInitializeComponents()override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* killer, class AActor* DamageCauser);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BeHitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* DeathAnimMontage;

private:
	//class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;

};