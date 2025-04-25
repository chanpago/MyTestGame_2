// OfficePlayerController.cpp

#include "OfficePlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h" 
#include "GameStartMenuBackGroundVideo.h"

AOfficePlayerController::AOfficePlayerController()
{
    CurrentCameraIndex = 0;
}

void AOfficePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 레벨에서 시네 카메라 액터를 찾기
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CineCameraActors);

    // 첫 번째 카메라로 전환
    if (CineCameraActors.Num() > 0)
    {
        SetViewTarget(CineCameraActors[0]);
        GetWorld()->GetTimerManager().SetTimer(CameraSwitchTimerHandle, this, &AOfficePlayerController::SwitchCamera, 6.0f, true);

        //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT(" CineCameraActors.Num() : %f"), CineCameraActors.Num()));
    }

   

}

void AOfficePlayerController::SwitchCamera()
{
    //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Switch")));
    if (CineCameraActors.Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CineCameraActors.Num() == 0")));
        return;
    }
    //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CineCameraActors.Num() != 0")));
    //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT(" CineCameraActors.Num() : %d"), CineCameraActors.Num()));
    CurrentCameraIndex = (CurrentCameraIndex + 1) % CineCameraActors.Num();
    SetViewTarget(CineCameraActors[CurrentCameraIndex]);


}

void AOfficePlayerController::StartGame()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("StartGame")));
    UWorld* World = GetWorld();
    if (World)
    {
        // 타이머 중지
        GetWorldTimerManager().ClearTimer(CameraSwitchTimerHandle);

        // 플레이어 스타트 위치를 찾기
        APlayerStart* PlayerStart = nullptr;
        for (TActorIterator<APlayerStart> It(World); It; ++It)
        {
            PlayerStart = *It;
            break; // 첫 번째 플레이어 스타트만 사용
        }

        if (PlayerStart)
        {
            FTransform SpawnTransform = PlayerStart->GetActorTransform();

            
        }
        else
        {
            //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("No PlayerStart found")));
        }

        // 시네 카메라 종료
        for (AActor* CameraActor : CineCameraActors)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CameraActor")));
            if (CameraActor)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CameraActor SetActorHiddenInGame")));
                CameraActor->SetActorHiddenInGame(true);
                CameraActor->SetActorTickEnabled(false);
                //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("CameraActor destroy")));
            }
        }

        // 플레이어의 카메라로 전환
        APawn* PlayerPawn = GetPawn();
        if (PlayerPawn)
        {
            SetViewTarget(PlayerPawn);
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Switched to Player Camera"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No PlayerPawn found"));
        }
    }
}