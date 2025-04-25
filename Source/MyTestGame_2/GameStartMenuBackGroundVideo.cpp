// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartMenuBackGroundVideo.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"



void UGameStartMenuBackGroundVideo::SetCameraTexture(UTextureRenderTarget2D* CameraTexture)
{
    if (BackgroundMaterialInstance && CameraTexture)
    {
        BackgroundMaterialInstance->SetTextureParameterValue(FName("TextureParameter"), CameraTexture);
    }
}

void UGameStartMenuBackGroundVideo::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackgroundMaterial)
    {
        BackgroundMaterialInstance = UMaterialInstanceDynamic::Create(BackgroundMaterial, this);
        if (BackgroundImage && BackgroundMaterialInstance)
        {
            BackgroundImage->SetBrushFromMaterial(BackgroundMaterialInstance);
        }
    }
}
