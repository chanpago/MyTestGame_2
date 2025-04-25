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

    // AMainHUD 포인터 가져오기
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
    }

    // 버튼 또는 다른 위젯 요소에 마우스 오버 이벤트를 바인딩합니다.
    BindMouseHoverEvents();

    // BackgroundMusic이 설정되어 있다면 재생을 시작합니다.
    StartBackgroundMusic();

    // Options 버튼 클릭 이벤트를 바인딩합니다.
    if (Options)
    {
        Options->OnClicked.AddDynamic(this, &UGameStartMenu::ShowOptionsPane);
    }

    // 처음에 OptionsPane을 숨깁니다.
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
        // 메뉴를 화면에 표시하고 마우스를 활성화합니다.
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
        // 메뉴를 숨기고 마우스를 비활성화합니다.
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
    // 버튼에 마우스 오버 이벤트를 바인딩합니다.
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
        Back->OnClicked.AddDynamic(this, &UGameStartMenu::OnBackButtonClicked);  // Back 버튼에 클릭 핸들러 바인딩
    }


    // 필요한 만큼 다른 버튼이나 위젯 요소에 대해서도 같은 방식으로 추가합니다.
}

void UGameStartMenu::ShowOptionsPane()
{
    if (OptionsPane)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ShowOptionsPane")));
        // OptionsPane의 가시성을 토글합니다.
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
