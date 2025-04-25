// Fill out your copyright notice in the Description page of Project Settings.

#include "MapSelect_Widget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UMapSelect_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 이벤트 바인딩
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
        // 여기서 이미지를 변경하거나 표시합니다.
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

    // 입력 모드를 게임으로 복원합니다.
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }
}