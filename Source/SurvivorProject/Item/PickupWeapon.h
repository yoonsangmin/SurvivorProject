// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupItem.h"
#include "PickupWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API APickupWeapon : public APickupItem
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* Other) override;

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<class UWeaponData> WeaponData;
};
