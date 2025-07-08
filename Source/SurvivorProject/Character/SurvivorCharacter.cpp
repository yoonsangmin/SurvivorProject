// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SurvivorCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/SurvivorPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SurvivorCharacterAttributeSet.h"
#include "UI/SurvivorWidgetComponent.h"

ASurvivorCharacter::ASurvivorCharacter()
{
	AbilitySystemComponent = nullptr;
	
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera")); 
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	TopDownCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;

	HpBar = CreateDefaultSubobject<USurvivorWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CameraRotationSpeed = 3.0f;
}

UAbilitySystemComponent* ASurvivorCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASurvivorCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASurvivorPlayerState* SurvivorPlayerState = GetPlayerState<ASurvivorPlayerState>();
	if (SurvivorPlayerState)
	{
		AbilitySystemComponent = SurvivorPlayerState->GetAbilitySystemComponent();
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(SurvivorPlayerState, this);

			const USurvivorCharacterAttributeSet* AttributeSet = AbilitySystemComponent->GetSet<USurvivorCharacterAttributeSet>();
			if (AttributeSet)
			{
				AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
			}

			
			for (const auto& StartAbility : StartAbilities)
			{
				if (StartAbility)
				{
					FGameplayAbilitySpec StartAbilitySpec(StartAbility);
					AbilitySystemComponent->GiveAbility(StartAbilitySpec);
				}
			}

			for (int32 i = 0; i < StartInputActionAndGameplayAbilities.Num(); ++i)
			{
				if (StartInputActionAndGameplayAbilities[i].GameplayAbility)
				{
					FGameplayAbilitySpec StartInputAbilitySpec(StartInputActionAndGameplayAbilities[i].GameplayAbility);
					StartInputAbilitySpec.InputID = i;
					AbilitySystemComponent->GiveAbility(StartInputAbilitySpec);
				}
			}
			UniqueInputID = StartInputActionAndGameplayAbilities.Num();
			
#ifdef WITH_EDITOR
			APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
			//PlayerController->ConsoleCommand(TEXT("ShowDebug AbilitySystem"));
#endif
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASurvivorCharacter::PossessedBy - PlayerState is not type of a ASurvivorPlayerState"));
	}
}

void ASurvivorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// 키보드 인풋용으로 반대 입력이 들어온 경우 막아주는 함수 - MoveAction보다 일찍 불려야 함
			EnhancedInputComponent->BindAction(ReverseMoveAction, ETriggerEvent::Triggered, this, &ASurvivorCharacter::ReverseMove);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivorCharacter::Move);
			EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &ASurvivorCharacter::RotateCamera);
			EnhancedInputComponent->BindAction(ToggleRotatingCameraAction, ETriggerEvent::Started, this, &ASurvivorCharacter::ToggleRotatingCamera);
			EnhancedInputComponent->BindAction(ToggleRotatingCameraAction, ETriggerEvent::Completed, this, &ASurvivorCharacter::ToggleRotatingCamera);
		}
	}

	SetupGASInputComponent();

	PlayerController->SetShowMouseCursor(true);
}

void ASurvivorCharacter::SetupGASInputComponent()
{
	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

			for (int32 i = 0; i < StartInputActionAndGameplayAbilities.Num(); ++i)
		{
			TriggeredBindingHandles.Emplace(i, EnhancedInputComponent->BindAction(StartInputActionAndGameplayAbilities[i].InputAction, ETriggerEvent::Triggered, this, &ASurvivorCharacter::GASInputPressed, i).GetHandle());
			CompletedBindingHandles.Emplace(i, EnhancedInputComponent->BindAction(StartInputActionAndGameplayAbilities[i].InputAction, ETriggerEvent::Completed, this, &ASurvivorCharacter::GASInputReleased, i).GetHandle());
			// TriggeredBindingHandles.Emplace(i, EnhancedInputComponent->BindAction(StartInputActionAndGameplayAbilities[i].InputAction, ETriggerEvent::Triggered, AbilitySystemComponent.Get(), &UAbilitySystemComponent::PressInputID, i).GetHandle());
			// CompletedBindingHandles.Emplace(i, EnhancedInputComponent->BindAction(StartInputActionAndGameplayAbilities[i].InputAction, ETriggerEvent::Completed, AbilitySystemComponent.Get(), &UAbilitySystemComponent::ReleaseInputID, i).GetHandle());
		}
	}
}

void ASurvivorCharacter::AddGASInput(UInputAction* InputAction, TSubclassOf<UGameplayAbility> GameplayAbility)
{
	if (!IsValid(InputAction) || !IsValid(GameplayAbility))
	{
		return;
	}

	if (!ensure(IsValid(AbilitySystemComponent)) || !ensure(IsValid(InputComponent)))
	{
		return;
	}
	
	FGameplayAbilitySpec InputAbilitySpec(GameplayAbility);
	InputAbilitySpec.InputID = UniqueInputID;
	AbilitySystemComponent->GiveAbility(InputAbilitySpec);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	TriggeredBindingHandles.Emplace(UniqueInputID, EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, AbilitySystemComponent.Get(), &UAbilitySystemComponent::PressInputID, UniqueInputID).GetHandle());
	CompletedBindingHandles.Emplace(UniqueInputID, EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, AbilitySystemComponent.Get(), &UAbilitySystemComponent::ReleaseInputID, UniqueInputID).GetHandle());

	++UniqueInputID;
}

void ASurvivorCharacter::RemoveGASInput(TSubclassOf<UGameplayAbility> GameplayAbility)
{
	if (!ensure(GameplayAbility))
	{
		return;
	}

	if (!ensure(IsValid(AbilitySystemComponent)) || !ensure(IsValid(InputComponent)))
	{
		return;
	}

	FGameplayAbilitySpec* InputAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(GameplayAbility);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->RemoveBindingByHandle(TriggeredBindingHandles[InputAbilitySpec->InputID]);
	EnhancedInputComponent->RemoveBindingByHandle(CompletedBindingHandles[InputAbilitySpec->InputID]);
	
	TriggeredBindingHandles.Remove(InputAbilitySpec->InputID);
	TriggeredBindingHandles.Remove(InputAbilitySpec->InputID);
	CompletedBindingHandles.Remove(InputAbilitySpec->InputID);
	
	AbilitySystemComponent->ClearAbility(InputAbilitySpec->Handle);
}

void ASurvivorCharacter::GASInputPressed(int32 InputId)
{
	AbilitySystemComponent->PressInputID(InputId);
}

void ASurvivorCharacter::GASInputReleased(int32 InputId)
{
	AbilitySystemComponent->ReleaseInputID(InputId);
}

// 키보드 인풋용으로 반대 입력이 들어온 경우 막아주는 함수
// 인풋 매핑 컨텍스트에서 반대 방향을 더 높은 우선 순위로 설정하고 같은 입력 처리를 한 번 더 함
void ASurvivorCharacter::ReverseMove(const FInputActionValue& Value)
{
	ReverseMovementVector = Value.Get<FVector2D>();
}

void ASurvivorCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>() + ReverseMovementVector;
	ReverseMovementVector = FVector2D::ZeroVector;
	float MovementVectorSizeSquared = MovementVector.SizeSquared();
	float MovementVectorSize = MovementVectorSizeSquared > 1.0f ? 1.0f : FMath::Sqrt(MovementVectorSizeSquared);
	FVector MovementDirection = FRotationMatrix::MakeFromX(FVector::VectorPlaneProject(TopDownCameraComponent->GetForwardVector(), FVector::UpVector)).Rotator().RotateVector(FVector(MovementVector.X, MovementVector.Y, 0.f)).GetSafeNormal();
	
	AddMovementInput(MovementDirection, MovementVectorSize);
}

void ASurvivorCharacter::RotateCamera(const FInputActionValue& Value)
{
	float MouseMovement = Value.Get<float>();
	UE_LOG(LogTemp, Display, TEXT("ASurvivorCharacter::MouseMove() %f"), MouseMovement);
	
	if (bIsRotatingCamera)
	{
		CameraBoom->AddRelativeRotation(FRotator(0.0f, MouseMovement * CameraRotationSpeed, 0.0f));

		// 무한 회전을 위해 마우스 포지션 초기화.
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		int32 SizeX, SizeY;
		PlayerController->GetViewportSize(SizeX, SizeY);
		PlayerController->SetMouseLocation(SizeX / 2, SizeY / 2);
	}
}

void ASurvivorCharacter::ToggleRotatingCamera(const FInputActionValue& Value)
{
	bIsRotatingCamera = Value.Get<bool>();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	PlayerController->SetShowMouseCursor(!bIsRotatingCamera);
}

void ASurvivorCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

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

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	AbilitySystemComponent->AddLooseGameplayTag(DeathTag);

	OnDead.ExecuteIfBound();
}

void ASurvivorCharacter::OnOutOfHealth()
{
	SetDead();
}

