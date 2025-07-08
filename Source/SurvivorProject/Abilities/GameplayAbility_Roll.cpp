// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_Roll.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/SurvivorAnimInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGameplayAbility_Roll::UGameplayAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility();
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	USkeletalMeshComponent* SkeletalMeshComoponent = Character->FindComponentByClass<USkeletalMeshComponent>();
	USurvivorAnimInstance* SurvivorAnimInstance = Cast<USurvivorAnimInstance>(SkeletalMeshComoponent->GetAnimInstance());
	if (!SurvivorAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Character's AnimInstance is not SurvivorAnimInstance."));
		CancelAbility();
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayerDodge"), SurvivorAnimInstance->GetAnimationData().Roll);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Roll::CompleteAbility);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Roll::CancelAbility);
	PlayMontageTask->ReadyForActivation();
}

void UGameplayAbility_Roll::CompleteAbility()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Roll::CancelAbility()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}