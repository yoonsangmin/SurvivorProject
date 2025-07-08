// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SurvivorUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void USurvivorUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

class UAbilitySystemComponent* USurvivorUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}
