// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "GameStartMenu.h"
#include "HpExpBar.h"
#include "MainHUD.h"
#include "OfficePlayerController.h"

//engine
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

void UGameStartMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // AMainHUD ������ ��������
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
    }

    // ��ư �Ǵ� �ٸ� ���� ��ҿ� ���콺 ���� �̺�Ʈ�� ���ε��մϴ�.
    BindMouseHoverEvents();

    // BackgroundMusic�� �����Ǿ� �ִٸ� ����� �����մϴ�.
    StartBackgroundMusic();

    // Options ��ư Ŭ�� �̺�Ʈ�� ���ε��մϴ�.
    if (Options)
    {
        Options->OnClicked.AddDynamic(this, &UGameStartMenu::ShowOptionsPane);
    }

    // ó���� OptionsPane�� ����ϴ�.
    if (OptionsPane)
    {
        OptionsPane->SetVisibility(ESlateVisibility::Collapsed);
    }

    

}

void UGameStartMenu::ShowMenu()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // �޴��� ȭ�鿡 ǥ���ϰ� ���콺�� Ȱ��ȭ�մϴ�.
        this->SetVisibility(ESlateVisibility::Visible);
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }
}

void UGameStartMenu::HideMenu()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // �޴��� ����� ���콺�� ��Ȱ��ȭ�մϴ�.
        this->SetVisibility(ESlateVisibility::Collapsed);

        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }
    if (BackgroundMusicComponent)
    {
        BackgroundMusicComponent->Stop();
        BackgroundMusicComponent = nullptr;
    }
    
}

void UGameStartMenu::PlayHoverSound()
{
    if (HoverSound)
    {
        UGameplayStatics::PlaySound2D(this, HoverSound);
    }
}

void UGameStartMenu::PlayClickSound()
{
    if (ClickSound)
    {
        UGameplayStatics::PlaySound2D(this, ClickSound);
    }
}

void UGameStartMenu::PlayStartSound()
{
    if (StartSound)
    {
        UGameplayStatics::PlaySound2D(this, StartSound);
    }
}

void UGameStartMenu::BindMouseHoverEvents()
{
    // ��ư�� ���콺 ���� �̺�Ʈ�� ���ε��մϴ�.
    if (Start)
    {
        Start->OnHovered.AddDynamic(this, &UGameStartMenu::PlayHoverSound);
        
        Start->OnClicked.AddDynamic(this, &UGameStartMenu::PlayStartSound);
        Start->OnClicked.AddDynamic(this, &UGameStartMenu::HideMenu);
        Start->OnClicked.AddDynamic(this, &UGameStartMenu::OnStartButtonClicked);

       
    }
    if (StartTourist)
    {
        StartTourist->OnHovered.AddDynamic(this, &UGameStartMenu::PlayHoverSound);
        StartTourist->OnClicked.AddDynamic(this, &UGameStartMenu::PlayClickSound);
    }
    if (PerformanceAnalysis)
    {
        PerformanceAnalysis->OnHovered.AddDynamic(this, &UGameStartMenu::PlayHoverSound);
        PerformanceAnalysis->OnClicked.AddDynamic(this, &UGameStartMenu::PlayClickSound);
    }
    if (Options)
    {
        Options->OnHovered.AddDynamic(this, &UGameStartMenu::PlayHoverSound);
        Options->OnClicked.AddDynamic(this, &UGameStartMenu::PlayClickSound);
    }
    if (Quit)
    {
        Quit->OnHovered.AddDynamic(this, &UGameStartMenu::PlayHoverSound);
        Quit->OnClicked.AddDynamic(this, &UGameStartMenu::PlayClickSound);
    }
    if (Back)
    {
        Back->OnHovered.AddDynamic(this, &UGameStartMenu::PlayHoverSound);
        Back->OnClicked.AddDynamic(this, &UGameStartMenu::OnBackButtonClicked);  // Back ��ư�� Ŭ�� �ڵ鷯 ���ε�
    }


    // �ʿ��� ��ŭ �ٸ� ��ư�̳� ���� ��ҿ� ���ؼ��� ���� ������� �߰��մϴ�.
}

void UGameStartMenu::ShowOptionsPane()
{
    if (OptionsPane)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ShowOptionsPane")));
        // OptionsPane�� ���ü��� ����մϴ�.
        ESlateVisibility CurrentVisibility = OptionsPane->GetVisibility();
        OptionsPane->SetVisibility(CurrentVisibility == ESlateVisibility::Visible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }
}

void UGameStartMenu::StartBackgroundMusic()
{
    if (BackgroundMusic)
    {
        BackgroundMusicComponent = UGameplayStatics::SpawnSound2D(this, BackgroundMusic);
    }
}


void UGameStartMenu::OnStartButtonClicked()
{

    if (MainHUD && MainHUD->HpExpWidget)
    {
        MainHUD->HpExpWidget->SetVisibility(ESlateVisibility::Visible);
    }

    //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("OnStartButtonClicked")));
    if (AOfficePlayerController* PlayerController = Cast<AOfficePlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("PlayerController OnStartButtonClicked")));
        PlayerController->StartGame();
    }
}

void UGameStartMenu::OnBackButtonClicked()
{
    if (OptionsPane)
    {
        OptionsPane->SetVisibility(ESlateVisibility::Collapsed);
    }
}
