// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_EquipWeapon.h"
#include "Tag/SurvivorTags.h"
#include "Item/WeaponData.h"
#include "Character/SurvivorCharacter.h"

UGameplayAbility_EquipWeapon::UGameplayAbility_EquipWeapon()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TAG_EVENT_CHARACTER_PICKUP_WEAPON;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(TriggerData);
}

void UGameplayAbility_EquipWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility();
		return;
	}
	
	UGameplayAbility::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UWeaponData* WeaponData = Cast<UWeaponData>(TriggerEventData->OptionalObject);
	if (!WeaponData)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayAbility_EquipWeapon::ActivateAbility - WeaponData is Missing!!"));
		CancelAbility();
		return;
	}

	ASurvivorCharacter* SurvivorCharacter = Cast<ASurvivorCharacter>(ActorInfo->AvatarActor.Get());
	if (!IsValid(SurvivorCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayAbility_EquipWeapon::ActivateAbility - AvatarActor is not SurvivorCharacter!!"));
		CancelAbility();
		return;
	}

	// 장착
	TMap<FName, TObjectPtr<UStaticMeshComponent>>& Weapons = SurvivorCharacter->GetWeapons();
	TObjectPtr<UStaticMeshComponent>* WeaponPtr = Weapons.Find(WeaponData->SocketToAttach);
	if (WeaponPtr && WeaponPtr->Get())
	{
		//TODO: 기존 무기 제거, 바인딩된 어빌리티 제거
		WeaponPtr->Get()->SetStaticMesh(WeaponData->WeaponMesh);
	}
	else
	{
		UStaticMeshComponent* WeaponMeshComponent = NewObject<UStaticMeshComponent>(SurvivorCharacter);
		WeaponMeshComponent->SetStaticMesh(WeaponData->WeaponMesh);
		WeaponMeshComponent->AttachToComponent(SurvivorCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponData->SocketToAttach);
		WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMeshComponent->RegisterComponent();
		Weapons.Add(WeaponData->SocketToAttach, WeaponMeshComponent);
	}

	USurvivorAnimInstance* SurvivorAnimInstance = Cast<USurvivorAnimInstance>(SurvivorCharacter->GetMesh()->GetAnimInstance());
	if (!SurvivorAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Character's AnimInstance is not SurvivorAnimInstance."));
		CancelAbility();
		return;
	}

	SurvivorAnimInstance->SetAnimationData(WeaponData->AnimationData);

	SurvivorCharacter->AddGASInput(WeaponData->AttackAction, WeaponData->AttackAbility);
	
	CompleteAbility();
}

void UGameplayAbility_EquipWeapon::CompleteAbility()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_EquipWeapon::CancelAbility()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

