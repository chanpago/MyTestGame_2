// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_InGame.h"
#include "UObject/ConstructorHelpers.h"
#include "MyBasicCharacter_2.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

AHUD_InGame::AHUD_InGame()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> Ui(TEXT("/Game/_My/UI/HpExpBar.HpExpBar_C"));
	if (Ui.Succeeded())
	{
		UiBPClass = Ui.Class;
		UE_LOG(LogTemp, Warning, TEXT("HUD loaded Successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD not loaded Successfully"));
	}
}

void AHUD_InGame::BeginPlay()
{
	Super::BeginPlay();
	if (UiBPClass)
	{
		UiWidget = CreateWidget<UUserWidget>(GetWorld(), UiBPClass);
		if (UiWidget)
		{
			UiWidget->AddToViewport();
		}
	}
}

void AHUD_InGame::SetHpBarValuePercent(float const value)
{
	HPBar->SetPercent(value);
}

void AHUD_InGame::SetExpBarValuePercent(float const value)
{
	ExpBar->SetPercent(value);
}

