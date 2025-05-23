// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterfaceBoss.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterfaceBoss : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYTESTGAME_2_API ICombatInterfaceBoss
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int Skill();
};
