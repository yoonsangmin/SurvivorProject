// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickupWeapon.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tag/SurvivorTags.h"
#include "WeaponData.h"

void APickupWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (WeaponData && WeaponData->WeaponMesh && Mesh)
	{
		Mesh->SetStaticMesh(WeaponData->WeaponMesh);
	}
}

void APickupWeapon::NotifyActorBeginOverlap(AActor* Other)
{
	if (!Other->ActorHasTag(TEXT("Player")))
	{
		return;
	}

	Super::NotifyActorBeginOverlap(Other);
	
	FGameplayEventData EventData;
	EventData.Instigator = Other;
	EventData.Target = this;
	EventData.OptionalObject = WeaponData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, TAG_EVENT_CHARACTER_PICKUP_WEAPON, EventData);
}
