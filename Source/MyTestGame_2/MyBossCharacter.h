// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/AnimMontage.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/Controller.h"
#include "CombatInterfaceBoss.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MyBossCharacter.generated.h"



class UNiagaraSystem;


UCLASS()
class MYTESTGAME_2_API AMyBossCharacter : public ACharacter, public ICombatInterfaceBoss
{
	GENERATED_BODY()
	

public:
	//=========================================================================
	// FUCTIONS
	//=========================================================================

	// Sets default values for this character's properties
	AMyBossCharacter(); 
    
	USkeletalMeshComponent* GetSpecificPawnMesh() const;

	FName GetMeshAttachPoint() const;

	USkeletalMeshComponent* GetSpecificPawnMesh2() const;

	FName GetMeshAttachPoint2() const;

	virtual void PostInitializeComponents()override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* killer, class AActor* DamageCauser);

	void SetCurrentMesh(class ABossAttach* NewMesh, class ABossAttach* LastMesh);

	void SetCurrentMesh(class ABossAttach2* NewMesh, class ABossAttach2* LastMesh);

	void EquipSkelMesh(class ABossAttach* SkelMesh);

	void EquipSkelMesh(class ABossAttach2* SkelMesh);

	UAnimMontage* GetMontage() const;

	int Skill_Implementation();

	void DeathAnimationEnd();

	void ShowDamageText(float DamageAmount);
	//****화면데미지

	UFUNCTION()
	void AnimNotify_SpawnNiagaraSystem(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	// 함수 추가
	UFUNCTION()
	void OnNiagaraCollisionEvent(FVector ParticlePosition, FVector ParticleVelocity, FVector ParticleSize, FName EventName);

	void HandleNiagaraCollision(FVector ParticlePosition, FVector ParticleVelocity, FVector ParticleSize, FName EventName);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SkillEnd();

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float BossHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MyState)
	float BossMaxHealth = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = Socket)
	FName MeshAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Socket2)
	FName MeshAttachPoint2;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UPlayerHitDamage> DamageTextWidgetClass;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BossBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara")
	UNiagaraSystem* NiagaraSystemTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Skill1MeleeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* Skill2MeleeAnim;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* BeHitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = Pawn)
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;


protected:
	//=========================================================================
	// FUCTIONS
	//=========================================================================
	void SpawnDefaultMesh();

	void SpawnDefaultMesh2();

	void AddMesh(class ABossAttach* MeshSkel);

	void AddMesh(class ABossAttach2* MeshSkel);

	//=========================================================================
	// PROPERTIES & VARIABLES
	//=========================================================================

	TArray<class ABossAttach*> SocketSkeletalMesh;

	TArray<class ABossAttach2*> SocketSkeletalMesh2;

	UPROPERTY(EditDefaultsOnly, Category = Socket)
	TArray<TSubclassOf<class ABossAttach>>DefaultMeshClass;

	UPROPERTY(EditDefaultsOnly, Category = Socket2)
	TArray<TSubclassOf<class ABossAttach2>>DefaultMeshClass2;

	class ABossAttach* CurrentMesh;

	class ABossAttach2* CurrentMesh2;

	class UWidgetComponent* BossHPWidgetComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
