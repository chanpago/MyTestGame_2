// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class MYTESTGAME_2_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;

	// Sets default values for this actor's properties
	AInterfaceTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void BeginFocus()override;
	virtual void EndFocus()override;
	virtual void BeginInteract()override;
	virtual void EndInteract()override;
	virtual void Interact(APlayerCharacter*PlayerCharacter)override;
};
