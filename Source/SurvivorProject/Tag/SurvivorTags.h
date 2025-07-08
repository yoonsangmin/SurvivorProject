#pragma once

#include "GameplayTagContainer.h"

#define TAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Character.AttackHit")) 
#define TAG_EVENT_CHARACTER_PICKUP_WEAPON FGameplayTag::RequestGameplayTag(TEXT("Event.Character.Pickup.Weapon"))
#define TAG_CHARACTER_STATE_ISDEAD FGameplayTag::RequestGameplayTag(TEXT("Character.State.IsDead"))