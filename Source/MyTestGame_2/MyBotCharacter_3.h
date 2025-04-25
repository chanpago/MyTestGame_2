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

	// 락온 위젯 컴포넌트
	UPROPERTY(EditDefaultsOnly, Category = "LockOn")
	UWidgetComponent* LockOnWidget;

	//그로기 스택
	//5초당 하나씩 삭제, 10이 되면 5초간 그로기 후 0 초기화
	float Groggy;

	bool IsGroggy = false;

	FTimerHandle GroggyTimerHandle; // Groggy 상태에서 일어나는 타이머 핸들러

	// 보스 이름 변수
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
	
	// 락온 위젯을 켜고 끄는 함수
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

	FTimerHandle GroggyResumeTimerHandle;  // GroggyHit 종료 후 GroggyStart를 재개하는 타이머 핸들러

	

};
