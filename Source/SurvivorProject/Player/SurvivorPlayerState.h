// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SurvivorPlayerState.generated.h"

class UAbilitySystemComponent;

UCLASS()
class SURVIVORPROJECT_API ASurvivorPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASurvivorPlayerState();
	
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

protected:
	UPROPERTY(VisibleAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = GAS)
	TObjectPtr<class USurvivorCharacterAttributeSet> AttributeSet;
};
