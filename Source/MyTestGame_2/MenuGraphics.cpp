//game
#include "MenuGraphics.h"

//engine
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"

void UMenuGraphics::NativeConstruct()
{
    Super::NativeConstruct();

    // 슬라이더 초기화 상태 로그 출력
    if (ResolutionQualitySlider)
    {
        UE_LOG(LogTemp, Log, TEXT("ResolutionQualitySlider is initialized with value: %f"), ResolutionQualitySlider->GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ResolutionQualitySlider is not initialized"));
    }

    // 슬라이더 이벤트 바인딩
    BindSliderEvents();

    // 초기 슬라이더 값 설정 및 텍스트 업데이트
    if (ResolutionQualitySlider) UpdateSliderText(ResolutionQualityText, ResolutionQualitySlider->GetValue());
    if (ViewDistanceSlider) UpdateSliderText(ViewDistanceText, ViewDistanceSlider->GetValue());
    if (AntiAliasingSlider) UpdateSliderText(AntiAliasingText, AntiAliasingSlider->GetValue());
    if (PostProcessingSlider) UpdateSliderText(PostProcessingText, PostProcessingSlider->GetValue());
    if (ShadowsSlider) UpdateSliderText(ShadowsText, ShadowsSlider->GetValue());
    if (TexturesSlider) UpdateSliderText(TexturesText, TexturesSlider->GetValue());
    if (EffectsSlider) UpdateSliderText(EffectsText, EffectsSlider->GetValue());
    if (FoliageSlider) UpdateSliderText(FoliageText, FoliageSlider->GetValue());
    if (ShadingSlider) UpdateSliderText(ShadingText, ShadingSlider->GetValue());
}

void UMenuGraphics::BindSliderEvents()
{
    if (ResolutionQualitySlider)
    {
        UE_LOG(LogTemp, Log, TEXT("ResolutionQualitySlider is valid"));
        ResolutionQualitySlider->SetMinValue(0.0f);
        ResolutionQualitySlider->SetMaxValue(100.0f);

        ResolutionQualitySlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnResolutionQualityChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ResolutionQualitySlider is null"));
    }

    if (ViewDistanceSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("ViewDistanceSlider is valid"));
        ViewDistanceSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnViewDistanceChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ViewDistanceSlider is null"));
    }

    if (AntiAliasingSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("AntiAliasingSlider is valid"));
        AntiAliasingSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnAntiAliasingChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AntiAliasingSlider is null"));
    }

    if (PostProcessingSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("PostProcessingSlider is valid"));
        PostProcessingSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnPostProcessingChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PostProcessingSlider is null"));
    }

    if (ShadowsSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("ShadowsSlider is valid"));
        ShadowsSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnShadowsChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShadowsSlider is null"));
    }

    if (TexturesSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("TexturesSlider is valid"));
        TexturesSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnTexturesChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TexturesSlider is null"));
    }

    if (EffectsSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("EffectsSlider is valid"));
        EffectsSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnEffectsChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EffectsSlider is null"));
    }

    if (FoliageSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("FoliageSlider is valid"));
        FoliageSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnFoliageChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FoliageSlider is null"));
    }

    if (ShadingSlider)
    {
        UE_LOG(LogTemp, Log, TEXT("ShadingSlider is valid"));
        ShadingSlider->OnValueChanged.AddDynamic(this, &UMenuGraphics::OnShadingChanged);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShadingSlider is null"));
    }
}

void UMenuGraphics::OnResolutionQualityChanged(float Value)
{
    
    int32 IntValue = FMath::RoundToInt(Value); // float 값을 정수로 변환
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    UserSettings->SetResolutionScaleNormalized(Value / 100.0f);  // 0.0~1.0 값으로 설정
    UserSettings->ApplySettings(false);
    UpdateSliderText(ResolutionQualityText, Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnViewDistanceChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetViewDistanceQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(ViewDistanceText, Value);
    UE_LOG(LogTemp, Log, TEXT("ViewDistanceSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnAntiAliasingChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetAntiAliasingQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(AntiAliasingText, Value);
    UE_LOG(LogTemp, Log, TEXT("AntiAliasingSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnPostProcessingChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetPostProcessingQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(PostProcessingText, Value);
    UE_LOG(LogTemp, Log, TEXT("PostProcessingSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnShadowsChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetShadowQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(ShadowsText, Value);
    UE_LOG(LogTemp, Log, TEXT("ShadowsSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnTexturesChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetTextureQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(TexturesText, Value);
    UE_LOG(LogTemp, Log, TEXT("TexturesSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnEffectsChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetVisualEffectQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(EffectsText, Value);
    UE_LOG(LogTemp, Log, TEXT("EffectsSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnFoliageChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetFoliageQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(FoliageText, Value);
    UE_LOG(LogTemp, Log, TEXT("FoliageSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::OnShadingChanged(float Value)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    int32 IntValue = FMath::RoundToInt(Value / 33.33f);  // 0~3 범위로 조정
    UserSettings->SetShadingQuality(IntValue);
    UserSettings->ApplySettings(false);
    UpdateSliderText(ShadingText, Value);
    UE_LOG(LogTemp, Log, TEXT("ShadingSlider Value Changed: %f"), Value);
    UGameplayStatics::PlaySound2D(this, SliderChangeSound);
}

void UMenuGraphics::UpdateSliderText(UTextBlock* TextBlock, float Value)
{
    if (TextBlock)
    {
        TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(Value))));
        UE_LOG(LogTemp, Log, TEXT("Updated Slider Text: %d%%"), FMath::RoundToInt(Value));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("error UpdateSliderText!"));
    }
}