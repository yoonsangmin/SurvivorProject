// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API UGameplayAbility_EquipWeapon : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_EquipWeapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	void CompleteAbility();
	void CancelAbility();
};
