// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Roll.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API UGameplayAbility_Roll : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Roll();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void CompleteAbility();

	UFUNCTION()
	void CancelAbility();
};
