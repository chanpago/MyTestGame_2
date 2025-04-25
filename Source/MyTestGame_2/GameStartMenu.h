// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "GameStartMenu.generated.h"

/**
 * 
 */


class AMainHUD;

UCLASS()
class MYTESTGAME_2_API UGameStartMenu : public UUserWidget
{
	GENERATED_BODY()

public:
    //=========================================================================
    // FUCTIONS
    //=========================================================================
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void ShowMenu();

    UFUNCTION(BlueprintCallable)
    void HideMenu();

    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnBackButtonClicked();

    //=========================================================================
    // PROPERTIES & VARIABLES
    //=========================================================================

    AMainHUD* MainHUD;

protected:
    //=========================================================================
    // PROPERTIES & VARIABLES
    //=========================================================================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* HoverSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* ClickSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* StartSound;

    UPROPERTY()
    UAudioComponent* BackgroundMusicComponent;

    //=========================================================================
    // FUCTIONS
    //=========================================================================
    UFUNCTION()
    void PlayHoverSound();

    UFUNCTION()
    void PlayClickSound();

    UFUNCTION()
    void PlayStartSound();

private:
    //=========================================================================
    // FUCTIONS
    //=========================================================================
    void BindMouseHoverEvents();

    UFUNCTION()
    void StartBackgroundMusic();

    UFUNCTION()
    void ShowOptionsPane();

    //=========================================================================
    // PROPERTIES & VARIABLES
    //=========================================================================

    UPROPERTY(meta = (BindWidget))
    class UButton* Start;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartTourist;

    UPROPERTY(meta = (BindWidget))
    class UButton* PerformanceAnalysis;

    UPROPERTY(meta = (BindWidget))
    class UButton* Options;

    UPROPERTY(meta = (BindWidget))
    class UButton* Quit;

    UPROPERTY(meta = (BindWidget))
    class UButton* Back;

    UPROPERTY(meta = (BindWidget))
    class UWidget* OptionsPane;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    USoundBase* BackgroundMusic;
};
