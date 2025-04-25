#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuGraphics.generated.h"

class USoundBase;
class UTextBlock;
class USlider;

UCLASS(Blueprintable)
class MYTESTGAME_2_API UMenuGraphics : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* SliderChangeSound;

private:
	void BindSliderEvents();

	UFUNCTION()
	void OnResolutionQualityChanged(float Value);

	UFUNCTION()
	void OnViewDistanceChanged(float Value);

	UFUNCTION()
	void OnAntiAliasingChanged(float Value);

	UFUNCTION()
	void OnPostProcessingChanged(float Value);

	UFUNCTION()
	void OnShadowsChanged(float Value);

	UFUNCTION()
	void OnTexturesChanged(float Value);

	UFUNCTION()
	void OnEffectsChanged(float Value);

	UFUNCTION()
	void OnFoliageChanged(float Value);

	UFUNCTION()
	void OnShadingChanged(float Value);

	void UpdateSliderText(UTextBlock* TextBlock, float Value);

	UPROPERTY(meta = (BindWidget))
	class USlider* ResolutionQualitySlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* ViewDistanceSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* AntiAliasingSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* PostProcessingSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* ShadowsSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* TexturesSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* EffectsSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* FoliageSlider;

	UPROPERTY(meta = (BindWidget))
	class USlider* ShadingSlider;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResolutionQualityText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ViewDistanceText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AntiAliasingText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PostProcessingText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShadowsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TexturesText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EffectsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FoliageText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ShadingText;
};