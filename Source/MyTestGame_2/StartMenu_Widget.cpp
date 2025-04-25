// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenu_Widget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UStartMenu_Widget::NativeConstruct()
{
	if (BtnStartGame)
	{
		BtnStartGame->OnClicked.AddDynamic(this, &UStartMenu_Widget::ButtonStartGameClicked);
	}
	if (BtnExitGame)
	{
		BtnExitGame->OnClicked.AddDynamic(this, &UStartMenu_Widget::ButtonExitGameClicked);
	}
}

void UStartMenu_Widget::ButtonStartGameClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/_My/123");
	
  
}

void UStartMenu_Widget::ButtonExitGameClicked()
{
	GetWorld()->GetFirstLocalPlayerFromController()->ConsoleCommand("quit");
}
