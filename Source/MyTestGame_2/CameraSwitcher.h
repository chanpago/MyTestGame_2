// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSwitcher.generated.h"

UCLASS()
class MYTESTGAME_2_API ACameraSwitcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraSwitcher();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
	TArray<AActor*> Cameras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> BackgroundWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UTextureRenderTarget2D* RenderTarget;

	int32 CurrentCameraIndex;
	double LastSwitchTime;

	UPROPERTY(EditAnywhere)
	class UGameStartMenuBackGroundVideo* BackgroundWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USceneCaptureComponent2D* SceneCaptureComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SwitchToNextCamera();

private:
	
};
