#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapSelect_Widget.h" // 위젯 클래스 포함
#include "Portal1.generated.h"



UCLASS()
class MYTESTGAME_2_API APortal1 : public AActor
{
	GENERATED_BODY()

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	// Sets default values for this actor's properties
	APortal1();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxComponent;


	UPROPERTY(EditAnywhere, Category = "Teleport")
	FName LevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MapSelectWidgetClass;  // 위젯 클래스의 타입을 저장하기 위한 변수

	UMapSelect_Widget* MapSelectWidget; // 위젯의 인스턴스를 저장하는 변수
};