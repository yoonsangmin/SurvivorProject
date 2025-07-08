// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_ComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API UGameplayAbility_ComboAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_ComboAttack();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnGameplayEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void CompleteAbility();

	UFUNCTION()
	void CancelAbility();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS)
	FString MontageSectionName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GAS, Meta = (Categories = Event))
	FGameplayTag ComboEventTag;

	TObjectPtr<class ACharacter> Character;
	bool bInputPressed = false;
};
