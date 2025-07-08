// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API UGameplayAbility_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnAttackCheckResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackBuffEffect;

	float CurrentLevel = 0;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class AGameplayAbilityTargetActor_AttackTrace> TargetActorClass;
};
