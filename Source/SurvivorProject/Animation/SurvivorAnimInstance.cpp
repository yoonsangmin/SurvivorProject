// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

USurvivorAnimInstance::USurvivorAnimInstance()
{
	MovingThreshold = 3.0f;
}

void USurvivorAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeInitializeAnimation - Character is null!"));
		return;
	}

	ResetAnimationData();
}

void USurvivorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character || !MovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeUpdateAnimation - Character or MovementComponent is null!"));
		return;
	}

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();
	bShouldMove = GroundSpeed > MovingThreshold;// && !MovementComponent->GetCurrentAcceleration().IsNearlyZero();
	bIsFalling = MovementComponent->IsFalling();

	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("USurvivorAnimInstance::NativeUpdateAnimation - AbilitySystemComponent is null!"));
		return;
	}
	
	ActiveGameplayTags = AbilitySystemComponent->GetOwnedGameplayTags();
}

void USurvivorAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	if (Character)
	{
		AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	}
}

void USurvivorAnimInstance::ResetAnimationData()
{
	OverrideAnimationData = BaseAnimationData;
}

void USurvivorAnimInstance::SetAnimationData(const FAnimationData& NewAnimationData)
{
	for (TFieldIterator<FProperty> PropIt(FAnimationData::StaticStruct()); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		if (Property)
		{
			if (FObjectProperty* ObjectPtrProp = CastField<FObjectProperty>(Property))
			{
				const void* NewValuePtr = ObjectPtrProp->ContainerPtrToValuePtr<void>(&NewAnimationData);
				UObject* NewObject = ObjectPtrProp->GetObjectPropertyValue(NewValuePtr);

				// nullptr인 경우 스킵
				if (!NewObject)
				{
					continue;
				}

				if (NewObject->IsA(UBlendSpace::StaticClass()))
				{
					UBlendSpace* NewBlendSpace = Cast<UBlendSpace>(NewObject);
					ObjectPtrProp->SetObjectPropertyValue_InContainer(&OverrideAnimationData, NewBlendSpace);
				}
				else if (NewObject->IsA(UAnimSequence::StaticClass()))
				{
					UAnimSequence* NewAnimSequence = Cast<UAnimSequence>(NewObject);
					ObjectPtrProp->SetObjectPropertyValue_InContainer(&OverrideAnimationData, NewAnimSequence);
				}
				else if (NewObject->IsA(UAnimMontage::StaticClass()))
				{
					UAnimMontage* NewAnimMontage = Cast<UAnimMontage>(NewObject);
					ObjectPtrProp->SetObjectPropertyValue_InContainer(&OverrideAnimationData, NewAnimMontage);
				}
			}
		}
	}
}