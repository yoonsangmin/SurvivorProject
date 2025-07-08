// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "SurvivorEnemyCharacter.generated.h"

DECLARE_DYNAMIC_DELEGATE (FOnEnemyDead);

UCLASS()
class SURVIVORPROJECT_API ASurvivorEnemyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASurvivorEnemyCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override; 

	virtual void Tick(float DeltaTime) override;
	
private:
	// Death
	void SetDead();

	UFUNCTION()
	void OnOutOfHealth();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<class USurvivorCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category = Combat, Meta = (Categories = Event))
	FGameplayTag AttackEventTag;
	
	// Death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Death)
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Death)
	FGameplayTag DeathTag;

	const float DeadEventDelayTime = 5.0f;

	FTimerHandle MoveTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death)
	FOnEnemyDead OnDead;
	
	// UI
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USurvivorWidgetComponent> HpBar;
};