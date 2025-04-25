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

    // �������� �ó� ī�޶� ���͸� ã��
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CineCameraActors);

    // ù ��° ī�޶�� ��ȯ
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
        // Ÿ�̸� ����
        GetWorldTimerManager().ClearTimer(CameraSwitchTimerHandle);

        // �÷��̾� ��ŸƮ ��ġ�� ã��
        APlayerStart* PlayerStart = nullptr;
        for (TActorIterator<APlayerStart> It(World); It; ++It)
        {
            PlayerStart = *It;
            break; // ù ��° �÷��̾� ��ŸƮ�� ���
        }

        if (PlayerStart)
        {
            FTransform SpawnTransform = PlayerStart->GetActorTransform();

            
        }
        else
        {
            //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("No PlayerStart found")));
        }

        // �ó� ī�޶� ����
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

        // �÷��̾��� ī�޶�� ��ȯ
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