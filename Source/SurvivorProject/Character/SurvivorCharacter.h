// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "SurvivorCharacter.generated.h"

DECLARE_DYNAMIC_DELEGATE (FOnCharacterDead);

USTRUCT()
struct FInputActionAndGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> InputAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TSubclassOf<class UGameplayAbility> GameplayAbility;
};

UCLASS()
class SURVIVORPROJECT_API ASurvivorCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASurvivorCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetupGASInputComponent();

	void AddGASInput(class UInputAction* InputAction, TSubclassOf<class UGameplayAbility> GameplayAbility);
	void RemoveGASInput(TSubclassOf<class UGameplayAbility> GameplayAbility);

	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);
	
	FORCEINLINE TMap<FName, TObjectPtr<UStaticMeshComponent>>& GetWeapons()
	{
		return Weapons;
	}
	
private:
	void Move(const FInputActionValue& Value);
	void ReverseMove(const FInputActionValue& Value);
	void RotateCamera(const FInputActionValue& Value);
	void ToggleRotatingCamera(const FInputActionValue& Value);

	// Death
	void SetDead();

	UFUNCTION()
	void OnOutOfHealth();
	
private:
	// GAS
	UPROPERTY(VisibleAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<FInputActionAndGameplayAbility> StartInputActionAndGameplayAbilities;
	
	TMap<int32, int32> TriggeredBindingHandles;
	TMap<int32, int32> CompletedBindingHandles;
	int32 UniqueInputID = 0;
	
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UStaticMeshComponent>> Weapons;
	
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// Movement
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ReverseMoveAction;

	FVector2D ReverseMovementVector;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> RotateCameraAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ToggleRotatingCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	float CameraRotationSpeed;
	
	bool bIsRotatingCamera = false;

	// Death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Death, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Death, Meta = (AllowPrivateAccess = "true"))
	FGameplayTag DeathTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Death, Meta = (AllowPrivateAccess = "true"))
	FOnCharacterDead OnDead;
	
	// UI
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USurvivorWidgetComponent> HpBar;
};