// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SurvivorWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVORPROJECT_API USurvivorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void InitWidget() override;
};
