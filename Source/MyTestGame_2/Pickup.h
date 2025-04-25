// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Pickup.generated.h"

class APlayerCharacter;
class UItemBase;
class UDataTable;

//class PlayerCharacter;

UCLASS()
class MYTESTGAME_2_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	

	//=========================================================================
	// FUCTIONS
	//=========================================================================
	APickup();

	void InitializePickup(const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus();
	virtual void EndFocus();


protected:
	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;


	//=========================================================================
	// FUCTIONS
	//=========================================================================
	virtual void BeginPlay() override;

	virtual void Interact(APlayerCharacter* PlayerCharcter)override;
	void UpdateInteractableData();

	void TakePickup(const APlayerCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)override;
#endif

};
