// Fill out your copyright notice in the Description page of Project Settings.

//game
#include "BTService_ChangeSpeed.h"
#include "MyBotCharacter_2.h"
#include "MyBotCharacter_3.h"
#include "MyBossCharacter.h"
#include "MyAIController.h"

//engine
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	auto const cont = owner_comp.GetAIOwner();
	if (cont->GetPawn()->IsA(AMyBotCharacter_2::StaticClass())) {
		AMyBotCharacter_2* const npcBot = Cast<AMyBotCharacter_2>(cont->GetPawn());
		npcBot->GetCharacterMovement()->MaxWalkSpeed = speed;
	}
	if (cont->GetPawn()->IsA(AMyBossCharacter::StaticClass())) {
		AMyBossCharacter* const npcBoss = Cast<AMyBossCharacter>(cont->GetPawn());
		npcBoss->GetCharacterMovement()->MaxWalkSpeed = speed;
	
	}
	if (cont->GetPawn()->IsA(AMyBotCharacter_3::StaticClass())) {
		AMyBotCharacter_3* const npcBot = Cast<AMyBotCharacter_3>(cont->GetPawn());
		npcBot->GetCharacterMovement()->MaxWalkSpeed = speed;
	}
	
	//AMyBossCharacter* const npcBoss = Cast<AMyBossCharacter>(cont->GetPawn());
	//npcBoss->GetCharacterMovement()->MaxWalkSpeed = speed;
}

FString UBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NpcBot speed");
}
