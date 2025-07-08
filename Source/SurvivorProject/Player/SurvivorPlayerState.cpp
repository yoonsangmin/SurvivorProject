// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivorPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SurvivorCharacterAttributeSet.h"

ASurvivorPlayerState::ASurvivorPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<USurvivorCharacterAttributeSet>(TEXT("AttributeSet"));
}
