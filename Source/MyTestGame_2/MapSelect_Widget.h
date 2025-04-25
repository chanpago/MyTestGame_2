// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelect_Widget.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UMapSelect_Widget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	// ��ư�� �̹��� ������Ʈ
	UPROPERTY(meta = (BindWidget))
	class UButton* TutorialButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GothicBossButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DeathWarBossButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* TestPlaceButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* PreviewImage;

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// ������ ������ �� ȣ��Ǵ� �Լ�
	virtual void NativeConstruct() override;
	
	// ��ư�� ȣ������ �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnTutorialButtonHovered();

	UFUNCTION()
	void OnGothicBossButtonHovered();

	UFUNCTION()
	void OnDeathWarBossButtonHovered();

	UFUNCTION()
	void OnTestPlaceButtonHovered();

	// ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnTutorialButtonClicked();

	UFUNCTION()
	void OnGothicBossButtonClicked();

	UFUNCTION()
	void OnDeathWarBossButtonClicked();

	UFUNCTION()
	void OnTestPlaceButtonClicked();

	UFUNCTION()
	void CloseMapSelectWidget();
};
