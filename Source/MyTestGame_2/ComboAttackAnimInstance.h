// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ComboAttackAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UComboAttackAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	


public:
	UFUNCTION()
	void AnimNotify_DamageStart();

	UFUNCTION()
	void AnimNotify_DamageEnd();

	UFUNCTION()
	void AnimNotify_RollStart();

	UFUNCTION()
	void AnimNotify_RollEnd();
};
