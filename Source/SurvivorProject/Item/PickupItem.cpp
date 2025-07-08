// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupItem.h"
#include "Components/SphereComponent.h"
#include "Tag/SurvivorTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

APickupItem::APickupItem()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->SetCollisionProfileName("Trigger");
	RootComponent = Trigger;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetupAttachment(Trigger);
}

class UAbilitySystemComponent* APickupItem::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APickupItem::NotifyActorBeginOverlap(class AActor* Other)
{
	if (!Other->ActorHasTag(TEXT("Player")))
	{
		return;
	}
	
	Super::NotifyActorBeginOverlap(Other);

	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);
	
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	SetLifeSpan(3.0f);
}

void APickupItem::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}

void APickupItem::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	AbilitySystemComponent->ExecuteGameplayCue(GameplayCueTag, Param);
}