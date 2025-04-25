//game
#include "CameraSwitcher.h"

//engine
#include "GameStartMenuBackGroundVideo.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
ACameraSwitcher::ACameraSwitcher()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentCameraIndex = 0;
    LastSwitchTime = 0.0;

    // Initialize SceneCaptureComponent2D
    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
    SceneCaptureComponent->SetupAttachment(RootComponent);
    SceneCaptureComponent->bCaptureEveryFrame = true;
    SceneCaptureComponent->bCaptureOnMovement = false;

    // Create a RenderTarget
    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    RenderTarget->InitAutoFormat(1920, 1080);
    SceneCaptureComponent->TextureTarget = RenderTarget;
}

// Called when the game starts or when spawned
void ACameraSwitcher::BeginPlay()
{
    Super::BeginPlay();

    if (Cameras.Num() > 0)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            PlayerController->SetViewTarget(Cameras[CurrentCameraIndex]);
        }

        if (BackgroundWidgetClass)
        {
            BackgroundWidget = CreateWidget<UGameStartMenuBackGroundVideo>(GetWorld(), BackgroundWidgetClass);
            if (BackgroundWidget)
            {
                BackgroundWidget->AddToViewport();

                UGameStartMenuBackGroundVideo* BackgroundWidgetInstance = Cast<UGameStartMenuBackGroundVideo>(BackgroundWidget);
                if (BackgroundWidgetInstance)
                {
                    BackgroundWidgetInstance->SetCameraTexture(RenderTarget);
                }
            }
        }
    }
}

// Called every frame
void ACameraSwitcher::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (FPlatformTime::Seconds() - LastSwitchTime > 5.0f)
    {
        SwitchToNextCamera();
        LastSwitchTime = FPlatformTime::Seconds();
    }
}

void ACameraSwitcher::SwitchToNextCamera()
{
    if (Cameras.Num() > 0)
    {
        CurrentCameraIndex = (CurrentCameraIndex + 1) % Cameras.Num();
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            PlayerController->SetViewTarget(Cameras[CurrentCameraIndex]);

            SceneCaptureComponent->SetWorldLocation(Cameras[CurrentCameraIndex]->GetActorLocation());
            SceneCaptureComponent->SetWorldRotation(Cameras[CurrentCameraIndex]->GetActorRotation());

            if (BackgroundWidget)
            {
                UGameStartMenuBackGroundVideo* BackgroundWidgetInstance = Cast<UGameStartMenuBackGroundVideo>(BackgroundWidget);
                if (BackgroundWidgetInstance)
                {
                    BackgroundWidgetInstance->SetCameraTexture(RenderTarget);
                }
            }
        }
    }
}