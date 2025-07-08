// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorWidgetComponent.h"
#include "SurvivorUserWidget.h"

void USurvivorWidgetComponent::InitWidget()
{
	Super::InitWidget();

	USurvivorUserWidget* SurvivorUserWidget = Cast<USurvivorUserWidget>(GetWidget());
	if (SurvivorUserWidget)
	{
		SurvivorUserWidget->SetAbilitySystemComponent(GetOwner());
	}
}
