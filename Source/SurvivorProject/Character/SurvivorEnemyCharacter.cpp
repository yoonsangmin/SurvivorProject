// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SurvivorEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Attributes/SurvivorCharacterAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/SurvivorWidgetComponent.h"

ASurvivorEnemyCharacter::ASurvivorEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<USurvivorCharacterAttributeSet>("AttributeSet");

	HpBar = CreateDefaultSubobject<USurvivorWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ASurvivorEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASurvivorEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivorEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AbilitySystemComponent->HasMatchingGameplayTag(DeathTag))
	{
		return;
	}
	
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Player)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			if (Player->GetDistanceTo(AIController->GetPawn()) > 100.0f)
			{
				AIController->MoveToActor(Player);
			}
			else if (AbilitySystemComponent)
			{
				AIController->StopMovement();
				// 공격
				FGameplayTagContainer AttackTagContainer;
				AttackTagContainer.AddTag(AttackEventTag);
				AbilitySystemComponent->TryActivateAbilitiesByTag(AttackTagContainer);
			}
		}
	}
}

void ASurvivorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	for (const auto& StartAbility : StartAbilities)
	{
		if (StartAbility)
		{
			FGameplayAbilitySpec StartAbilitySpec(StartAbility);
			AbilitySystemComponent->GiveAbility(StartAbilitySpec);
		}
	}
}

void ASurvivorEnemyCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	MoveTimerHandle.Invalidate();
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("ASurvivorEnemyCharacter::SetDead - AnimInstance is null"));
	}
	else if (DeathMontage && AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->Montage_Play(DeathMontage, 1.0f);
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);

	AbilitySystemComponent->AddLooseGameplayTag(DeathTag);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
	}

	OnDead.ExecuteIfBound();
}

void ASurvivorEnemyCharacter::OnOutOfHealth()
{
	SetDead();
}
