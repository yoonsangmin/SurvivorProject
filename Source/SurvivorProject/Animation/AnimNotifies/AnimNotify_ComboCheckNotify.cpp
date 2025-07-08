// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ComboCheckNotify.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_ComboCheckNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();
		if (IsValid(Owner))
		{
			FGameplayEventData Payload;
			Payload.EventMagnitude = NextComboIndex;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, Payload);
		}
	}
}
