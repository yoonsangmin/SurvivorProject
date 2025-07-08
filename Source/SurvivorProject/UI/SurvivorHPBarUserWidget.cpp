// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SurvivorHPBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SurvivorCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USurvivorHPBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(USurvivorCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &USurvivorHPBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(USurvivorCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &USurvivorHPBarUserWidget::OnMaxHealthChanged);

		const USurvivorCharacterAttributeSet* AttributeSet = ASC->GetSet<USurvivorCharacterAttributeSet>();
		if (AttributeSet)
		{
			CurrentHealth = AttributeSet->GetHealth();
			CurrentMaxHealth = AttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void USurvivorHPBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void USurvivorHPBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void USurvivorHPBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
