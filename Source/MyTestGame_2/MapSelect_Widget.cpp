// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSelect_Widget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UMapSelect_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ư �̺�Ʈ ���ε�
    if (TutorialButton)
    {
        TutorialButton->OnHovered.AddDynamic(this, &UMapSelect_Widget::OnTutorialButtonHovered);
        TutorialButton->OnClicked.AddDynamic(this, &UMapSelect_Widget::OnTutorialButtonClicked);
    }

    if (GothicBossButton)
    {
        GothicBossButton->OnHovered.AddDynamic(this, &UMapSelect_Widget::OnGothicBossButtonHovered);
        GothicBossButton->OnClicked.AddDynamic(this, &UMapSelect_Widget::OnGothicBossButtonClicked);
    }

    if (DeathWarBossButton)
    {
        DeathWarBossButton->OnHovered.AddDynamic(this, &UMapSelect_Widget::OnDeathWarBossButtonHovered);
        DeathWarBossButton->OnClicked.AddDynamic(this, &UMapSelect_Widget::OnDeathWarBossButtonClicked);
    }

    if (TestPlaceButton)
    {
        TestPlaceButton->OnHovered.AddDynamic(this, &UMapSelect_Widget::OnTestPlaceButtonHovered);
        TestPlaceButton->OnClicked.AddDynamic(this, &UMapSelect_Widget::OnTestPlaceButtonClicked);
    }
}

void UMapSelect_Widget::OnTutorialButtonHovered()
{
    if (PreviewImage)
    {
        // ���⼭ �̹����� �����ϰų� ǥ���մϴ�.
        UTexture2D* ImageToShow = LoadObject<UTexture2D>(nullptr, TEXT("'/Game/_My/UI/Images/TutorialImage.TutorialImage'"));
        if (ImageToShow)
        {
            PreviewImage->SetBrushFromTexture(ImageToShow);
        }
    }
}

void UMapSelect_Widget::OnGothicBossButtonHovered()
{
    if (PreviewImage)
    {
        UTexture2D* ImageToShow = LoadObject<UTexture2D>(nullptr, TEXT("'/Game/_My/UI/Images/GothicBossImage.GothicBossImage'"));
        if (ImageToShow)
        {
            PreviewImage->SetBrushFromTexture(ImageToShow);
        }
    }
}

void UMapSelect_Widget::OnDeathWarBossButtonHovered()
{
    if (PreviewImage)
    {
        UTexture2D* ImageToShow = LoadObject<UTexture2D>(nullptr, TEXT("'/Game/_My/UI/Images/DeathWarBossImage.DeathWarBossImage'"));
        if (ImageToShow)
        {
            PreviewImage->SetBrushFromTexture(ImageToShow);
        }
    }
}

void UMapSelect_Widget::OnTestPlaceButtonHovered()
{
    if (PreviewImage)
    {
        UTexture2D* ImageToShow = LoadObject<UTexture2D>(nullptr, TEXT("'/Game/_My/UI/Images/TestPlaceImage.TestPlaceImage'"));
        if (ImageToShow)
        {
            PreviewImage->SetBrushFromTexture(ImageToShow);
        }
    }
}

void UMapSelect_Widget::OnTutorialButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("Level_Tutorial"));
    CloseMapSelectWidget();
}

void UMapSelect_Widget::OnGothicBossButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("GothicField_BOSS"));
    CloseMapSelectWidget();
}

void UMapSelect_Widget::OnDeathWarBossButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("DeathWarBoss"));
    CloseMapSelectWidget();
}

void UMapSelect_Widget::OnTestPlaceButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("Level1"));
    CloseMapSelectWidget();
}

void UMapSelect_Widget::CloseMapSelectWidget()
{
    RemoveFromViewport();

    // �Է� ��带 �������� �����մϴ�.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }
}