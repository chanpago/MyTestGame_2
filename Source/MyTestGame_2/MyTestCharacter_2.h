// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBasicCharacter_2.h"
#include "MyTestCharacter_2.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API AMyTestCharacter_2 : public AMyBasicCharacter_2
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AMyTestCharacter_2();

	virtual void PostInitializeComponents()override;

	void AddEXP(float EXP);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float MyPlayerHudHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float MyPlayerHudMaxHealth = MyMaxHealth;

	class UWidgetComponent* WidgetComponent;

	class UWidgetComponent* BossHPWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MyEXPnum;

	//void Attack_Melee();

	//void ShowDamageText(float DamageAmount);
	//****화면데미지

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)override;
	void MoveForward(float value);
	void MoveRight(float value);

	void OpenInventory();

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();


	UUserWidget* DamageTextWidget;


private:
	class UAIPerceptionStimuliSourceComponent* stimulus;
	void SetupStimulus();

	bool bRightMouseClicked;
	void OnRightMouseClick();
	void OnRightMouseRelease();
	void Turn(float Value);
	void LookUp(float Value);

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float TurnRate = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float LookUpRate = 45.0f;
};
