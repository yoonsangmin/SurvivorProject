// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// 임시로 사용
#include "Animation/SurvivorAnimInstance.h"

#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 메시
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<UStaticMesh> WeaponMesh;

	// 소켓 이름
	UPROPERTY(EditAnywhere, Category = Weapon)
	FName SocketToAttach = TEXT("weapon_socket");
	
	// 변경할 애니메이션
	UPROPERTY(EditAnywhere, Category = Animation)
	FAnimationData AnimationData;

	// 공격에 사용할 어빌리티
	UPROPERTY(EditAnywhere, Category = "GAS|Input")
	TObjectPtr<class UInputAction> AttackAction;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<class UGameplayAbility> AttackAbility;
};
