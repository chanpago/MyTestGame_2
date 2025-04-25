// OfficePlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "GameStartMenuBackGroundVideo.h"
#include "OfficePlayerController.generated.h"

/**
 *
 */
UCLASS()
class MYTESTGAME_2_API AOfficePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AOfficePlayerController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UGameStartMenuBackGroundVideo> MainMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UMaterial* RenderTargetMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTextureRenderTarget2D* RenderTarget;

    void StartGame();


    UPROPERTY(EditAnywhere)
    TArray<AActor*> CineCameraActors;
protected:
    virtual void BeginPlay() override;



private:
    void SwitchCamera();

   



    int32 CurrentCameraIndex;

    FTimerHandle CameraSwitchTimerHandle;

    // 저장할 UI 위젯 참조
    UPROPERTY()
    UGameStartMenuBackGroundVideo* MainMenuWidget;

    
};