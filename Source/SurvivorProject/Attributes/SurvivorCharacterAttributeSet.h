// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SurvivorCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOutOfHealthDelegate);

UCLASS()
class SURVIVORPROJECT_API USurvivorCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USurvivorCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, AttackPower);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, MaxAttackPower);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USurvivorCharacterAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	mutable FOutOfHealthDelegate OnOutOfHealth;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData MaxAttackRadius;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData AttackPower;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData MaxAttackPower;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (HideFromModifiers))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Damage;

	bool bOutOfHealth;
};
