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
	// 버튼과 이미지 컴포넌트
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
	// 위젯이 생성될 때 호출되는 함수
	virtual void NativeConstruct() override;
	
	// 버튼이 호버링될 때 호출되는 함수
	UFUNCTION()
	void OnTutorialButtonHovered();

	UFUNCTION()
	void OnGothicBossButtonHovered();

	UFUNCTION()
	void OnDeathWarBossButtonHovered();

	UFUNCTION()
	void OnTestPlaceButtonHovered();

	// 버튼이 클릭될 때 호출되는 함수
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
