// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_ComboAttack.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGameplayAbility_ComboAttack::UGameplayAbility_ComboAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility();
		return;
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Character = CastChecked<ACharacter>(ActorInfo->AvatarActor);
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayComboAttack"), ComboAttackMontage);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_ComboAttack::CompleteAbility);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_ComboAttack::CompleteAbility);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_ComboAttack::CancelAbility);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_ComboAttack::CancelAbility);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent*  WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ComboEventTag);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UGameplayAbility_ComboAttack::OnGameplayEventReceived);
	WaitGameplayEventTask->ReadyForActivation();
}

void UGameplayAbility_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	bInputPressed = true;
}

void UGameplayAbility_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_ComboAttack::OnGameplayEventReceived(FGameplayEventData Payload)
{
	if (!bInputPressed)
	{
		return;
	}

	const int32 CurrentMontageSectionIndex = static_cast<int32>(Payload.EventMagnitude); 
	FName NextMontageSectionName = *FString::Printf(TEXT("%s%d"), *MontageSectionName, CurrentMontageSectionIndex);
	Character->GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextMontageSectionName);

	bInputPressed = false;
}

void UGameplayAbility_ComboAttack::CompleteAbility()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_ComboAttack::CancelAbility()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
