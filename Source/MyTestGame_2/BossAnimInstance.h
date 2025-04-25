// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Groggy")
	bool IsHitWhileGroggy;
};
