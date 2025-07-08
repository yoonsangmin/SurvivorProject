// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Tasks/AbilityTask_AttackHitCheck.h"
#include "Abilities/GameplayAbilityTargetActor_AttackTrace.h"
#include "AbilitySystemComponent.h"

UAbilityTask_AttackHitCheck* UAbilityTask_AttackHitCheck::CreateTask(UGameplayAbility* OwningAbility,
	TSubclassOf<AGameplayAbilityTargetActor_AttackTrace> TargetActorClass)
{
	UAbilityTask_AttackHitCheck* NewTask = NewAbilityTask<UAbilityTask_AttackHitCheck>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UAbilityTask_AttackHitCheck::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UAbilityTask_AttackHitCheck::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
	
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_AttackHitCheck::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor_AttackTrace>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->bDebug = true;
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UAbilityTask_AttackHitCheck::OnTargetDataReadyCallback);
	}
}

void UAbilityTask_AttackHitCheck::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UAbilityTask_AttackHitCheck::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
