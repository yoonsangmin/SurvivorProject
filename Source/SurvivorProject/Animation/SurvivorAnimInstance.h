// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SurvivorAnimInstance.generated.h"

class UAnimSequence;
class UAnimMontage;
class UBlendSpace;
USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UBlendSpace> Walk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimSequence> Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimSequence> Fall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimSequence> Land;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> Roll;
};

UCLASS()
class SURVIVORPROJECT_API USurvivorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USurvivorAnimInstance();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeBeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Animation)
	void ResetAnimationData();

	UFUNCTION(BlueprintCallable, Category = Animation)
	FORCEINLINE FAnimationData& GetAnimationData()
	{
		return OverrideAnimationData;
	}

	UFUNCTION(BlueprintCallable, Category = Animation)
	void SetAnimationData(const FAnimationData& NewAnimationData);

protected:
	// 캐시
	UPROPERTY(BlueprintReadOnly, Category = Animation)
	TObjectPtr<class ACharacter> Character;
	
	UPROPERTY(BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	// 애니메이션
	UPROPERTY(EditAnywhere, Category = Animation)
	FAnimationData BaseAnimationData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	FAnimationData OverrideAnimationData;

	// 상태
	UPROPERTY(BlueprintReadOnly, Category = Animation)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = Animation)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Animation)
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = Animation)
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Animation)
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	float MovingThreshold;
};
