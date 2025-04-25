// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenu_HUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

AStartMenu_HUD::AStartMenu_HUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget>ui(TEXT("/Game/_My/UI/StartMenu_BP"));
	if (ui.Succeeded())
	{
		UiBPClass = ui.Class;
	}
}

void AStartMenu_HUD::BeginPlay()
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
