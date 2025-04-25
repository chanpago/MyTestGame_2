// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyTestGame_2GameModeBase.h"
#include "Blueprint/UserWidget.h"

void AMyTestGame_2GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMyTestGame_2GameModeBase::ChangeUI()
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	CurrentWidget->AddToViewport();
}
