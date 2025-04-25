// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameStartMenuBackGroundVideo.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTGAME_2_API UGameStartMenuBackGroundVideo : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UFUNCTION(BlueprintCallable)
    void SetCameraTexture(UTextureRenderTarget2D* CameraTexture);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    class UImage* BackgroundImage;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    UMaterialInterface* BackgroundMaterial;

    UMaterialInstanceDynamic* BackgroundMaterialInstance;
};
