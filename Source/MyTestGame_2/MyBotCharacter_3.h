// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBasicCharacter_2.h"
#include "Animation/AnimMontage.h"
#include "CombatInterface.h"
#include "MyBotCharacter_3.generated.h"

/**
 * 
 */
class UWidgetComponent;

UCLASS()
class MYTESTGAME_2_API AMyBotCharacter_3 : public AMyBasicCharacter_2, public ICombatInterface
{
	GENERATED_BODY()

public:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior2;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* BeHitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* Skill1AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* lightHitedAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* BlockAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DashAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* BossSkill3AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* WalkingAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* GroggyStart;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* GroggyEnd;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* GroggyHit;

	// ���� ���� ������Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	UWidgetComponent* LockOnWidget;

	//�׷α� ����
	//5�ʴ� �ϳ��� ����, 10�� �Ǹ� 5�ʰ� �׷α� �� 0 �ʱ�ȭ
	float Groggy;

	bool IsGroggy = false;

	FTimerHandle GroggyTimerHandle; // Groggy ���¿��� �Ͼ�� Ÿ�̸� �ڵ鷯

	// ���� �̸� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FText BossName;

	class UWidgetComponent* BossHPWidgetComponent;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UBossWidget* BossWidget;


	//=========================================================================
	// FUCTIONS
	//=========================================================================

	AMyBotCharacter_3();

	int MeleeAttack_Implementation()override;

	UFUNCTION(BlueprintCallable)
	void MeleeAttackEnd();

	UAnimMontage* GetMontage() const;

	virtual void PostInitializeComponents()override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* killer, class AActor* DamageCauser);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
	
	// ���� ������ �Ѱ� ���� �Լ�
	void SetLockOnWidgetVisibility(bool bIsVisible);

	void SetCanDamaged(bool value);

	bool GetCanDamaged();

	void GroggyState();

	void GroggyEndState();

	void TakeDamageWhileGroggy();

	void ShowBossUIDisplay();
	
	void HideBossUIDisplay();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;

	bool CanDamaged = false;

	FTimerHandle GroggyResumeTimerHandle;  // GroggyHit ���� �� GroggyStart�� �簳�ϴ� Ÿ�̸� �ڵ鷯

	

};
